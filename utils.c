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

static void	free_paths(char **paths, char *p0, char *p1)
{
	size_t	i;

	if (paths != NULL)
	{
		i = 0;
		while (paths[i] != NULL)
		{
			free(paths[i]);
			i++;
		}
		free(paths);
	}
	if (p0)
		free(p0);
	if (p1)
		free(p1);
}

static char	*try_path(char *dir, char *cmd, char **paths)
{
	char	*p0;
	char	*p1;

	p0 = ft_strjoin(dir, "/");
	if (!p0)
	{
		free_paths(paths, NULL, NULL);
		return (werror("\x1b[91mERROR: ft_strjoin(p0)\x1b[0m\n"));
	}
	p1 = ft_strjoin(p0, cmd);
	if (!p1)
	{
		free_paths(paths, p0, NULL);
		return (werror("\x1b[91mERROR: ft_strjoin(p1)\x1b[0m\n"));
	}
	if (access(p1, X_OK) == 0)
	{
		free_paths(paths, p0, NULL);
		return (p1);
	}
	free_paths(NULL, p0, p1);
	return (NULL);
}

char	*path(char *cmd, char **envp)
{
	char	**paths;
	size_t	i;
	char	*res;

	i = 0;
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5))
		i++;
	if (!envp[i])
		return (werror("\x1b[91mERROR: PATH not found\x1b[0m\n"));
	paths = ft_split(envp[i] + 5, ':');
	if (!paths || !paths[0])
		return (werror("\x1b[91mERROR: PATH not found\x1b[0m\n"));
	i = 0;
	while (paths[i])
	{
		res = try_path(paths[i], cmd, paths);
		if (res)
			return (res);
		i++;
	}
	free_paths(paths, NULL, NULL);
	return (werror("\x1b[91mERROR: Command not found in PATH\x1b[0m\n"));
}
