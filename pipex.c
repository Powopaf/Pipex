/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 01:24:36 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/21 11:07:50 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exec_cmd(char *cmd, char **arg, char **envp)
{
	int	i;

	execve(cmd, arg, envp);
	perror("\x1b[91mError: execve()\x1b[0m");
	i = 0;
	while (arg[i] != NULL)
	{
		free(arg[i]);
		i++;
	}
	free(arg);
	exit(1);
}

static void	command2(char *file2, char *cmd2, char **envp, int pip[2])
{
	int		fd;
	char	**cmd;

	fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("\x1b[91mError: open(file2)\x1b[0m");
		exit(1);
	}
	if (dup2(pip[0], STDIN_FILENO) == -1)
	{
		perror("\x1b[91mError: dup2(pip0, in)\x1b[0m");
		close(fd);
		close(pip[0]);
		close(pip[1]);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("\x1b[91mError: dup2(fd, out)\x1b[0m");
		close(fd);
		close(pip[0]);
		close(pip[1]);
		exit(1);
	}
	close(fd);
	close(pip[0]);
	close(pip[1]);
	cmd = ft_split(cmd2, ' ');
	if (!cmd || !*cmd)
	{
		write(2, "\x1b[91mERROR: ft_split(cmd2)\x1b[0m\n",
			ft_strlen("\x1b[91mERROR: ft_split(cmd2)\x1b[0m\n"));
		exit(1);
	}
	exec_cmd(cmd[0], cmd, envp);
	exit(1);
}

static void	command1(char *file1, char *cmd1, char **envp, int pip[2])
{
	int		fd;
	char	**cmd;

	fd = open(file1, O_RDONLY);
	if (fd == -1)
	{
		perror("\x1b[91mError: open(file1)\x1b[0m");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("\x1b[91mError: dup2(fd, in)\x1b[0m");
		close(fd);
		close(pip[0]);
		close(pip[1]);
		exit(1);
	}
	if (dup2(pip[1], STDOUT_FILENO) == -1)
	{
		perror("\x1b[91mError: dup2(pip, out)\x1b[0m");
		close(fd);
		close(pip[0]);
		close(pip[1]);
		exit(1);
	}
	close(fd);
	close(pip[0]);
	close(pip[1]);
	cmd = ft_split(cmd1, ' ');
	if (!cmd || !*cmd)
	{
		write(2, "\x1b[91mERROR: ft_split(cmd1)\x1b[0m\n",
			ft_strlen("\x1b[91mERROR: ft_split(cmd1)\x1b[0m\n"));
		exit(1);
	}
	exec_cmd(cmd[0], cmd, envp);
	exit(1);
}

int	main(int argc, char **argv, char **envp)
{
	int		pip[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
	{
		write(2, "Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 46);
		return (1);
	}
	if (pipe(pip) == -1)
	{
		perror("\x1b[91mError: pipe()\x1b[0m");
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
		exit(error1(pip[0], pip[1], "\x1b[91mError: first fork()\x1b[0m"));
	if (pid1 == 0)
		command1(argv[1], argv[2], envp, pip);
	pid2 = fork();
	if (pid2 == -1)
		exit(error2(pip[0], pip[1], "\x1b[91mError: second fork()\x1b[0m", pid1));
	if (pid2 == 0)
		command2(argv[4], argv[3], envp, pip);
	close(pip[0]);
	close(pip[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
