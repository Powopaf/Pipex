/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 01:24:36 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/20 15:24:52 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	father(char *file2, char *cmd2, char **envp, int pip[2])
{
}

static void	child(char *file1, char *cmd1, char **envp, int pip[2])
{
	int	err;

	err = open(file1, 
}

int	main(int argv, char **argc, char** envp)
{
	int		pip[2];
	pid_t	pid;

	if (argv != 5)
	{
		write(1, "Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 45);
		return (EXIT_FAILURE);
	}
	pid = fork();
	pipe(pip);
	if (pid == -1)
	{
		perror("fork erorr\n");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
		child(argc[1], argc[2], envp, pip);
	waitpid(pid, NULL, 0);
	father(argc[4], argc[3], envp, pip);

	return (EXIT_SUCCESS);
}
