/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:17:29 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/21 11:07:00 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	error1(int fd1, int fd2, char *err)
{
	perror(err);
	close(fd1);
	close(fd2);
	return (1);
}

int	error2(int pip[2], char *err, pid_t pid)
{
	perror(err);
	close(pip[0]);
	close(pip[1]);
	waitpid(pid, NULL, 0);
	return (1);
}

void	*werror(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (NULL);
}

int	error3(char *msg, int fd1, int pip[2])
{
	perror(msg);
	close(fd1);
	close(pip[0]);
	close(pip[1]);
	return (1);
}

int	errorp(char *msg)
{
	perror(msg);
	return (1);
}
