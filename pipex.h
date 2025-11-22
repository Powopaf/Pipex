/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <pifourni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:16:16 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/21 11:06:46 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include "libft/libft.h"

int		error1(int fd1, int fd2, char *err);
int		error2(int pip[2], char *err, pid_t pid);
char	*path(char *cmd, char **envp);
void	*werror(const char *msg);
int		error3(char *msg, int fd1, int pip[2]);
int		errorp(char *msg);
int		exit_status(pid_t pid1, pid_t pid2);

#endif
