/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:45:54 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/21 16:45:56 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_paths(char **paths)
{
	size_t	i;

	i = 0;
	while (paths[i] != NULL)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*path(char *cmd, char **envp)
{
	char	**paths;
	size_t	i;
	char	*p0;
	char	*p1;
	int		fd;

	i = 0;
	while (envp[i] != NULL && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	if (!paths || !paths[0])
	{
		write(2, "\x1b[91mERROR: ft_split(PATH)\x1b[0m\n",
			ft_strlen("\x1b[91mERROR: ft_split(PATH)\x1b[0m\n"));
		return (NULL);
	}
	i = 0;
	while (paths[i] != NULL)
	{
		p0 = ft_strjoin(paths[i], "/");
		if (p0 == NULL)
		{
			write(2, "\x1b[91mERROR: ft_strjoin(p0)\x1b[0m\n",
				ft_strlen("\x1b[91mERROR: ft_strjoin(p0)\x1b[0m\n"));
			free_paths(paths);
			return (NULL);
		}
		p1 = ft_strjoin(p0, cmd);
		if (p1 == NULL)
		{
			write(2, "\x1b[91mERROR: ft_strjoin(p1)\x1b[0m\n",
				ft_strlen("\x1b[91mERROR: ft_strjoin(p1)\x1b[0m\n"));
			free(p0);
			free_paths(paths);
			return (NULL);
		}
		fd = open(p1, O_RDONLY);
		if (fd != -1)
		{
			close(fd);
			free(p0);
			free_paths(paths);
			return (p1);
		}
		free(p0);
		free(p1);
		i++;
	}
	free_paths(paths);
	write(2, "\x1b[91mERROR: command not found\x1b[0m\n",
		ft_strlen("\x1b[91mERROR: command not found\x1b[0m\n"));
	return (NULL);
}
