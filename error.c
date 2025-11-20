/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:17:29 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/20 17:18:37 by pifourni         ###   ########.fr       */
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

int error2(int fd1, int fd2, char *err, pid_t pid)
{
	perror(err);
	close(fd1);
	close(fd2);
	waitpid(pid, NULL, 0);
	return (1);
}
