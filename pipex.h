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

char	**ft_split(char const *s, char c);
size_t	ft_strlen(char *s);
int		error1(int fd1, int fd2, char *err);
int		error2(int fd1, int fd2, char *err, pid_t pid);

#endif
