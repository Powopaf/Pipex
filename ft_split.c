/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pifourni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:59:51 by pifourni          #+#    #+#             */
/*   Updated: 2025/11/20 16:06:24 by pifourni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	len(char const *str, char c, int i)
{
	size_t	l;

	l = 0;
	while (str[i + l] != '\0' && str[i + l] != c)
		l++;
	return (l);
}

static int	count_word(char const *str, char c)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] == c)
			i++;
		if (str[i] == '\0')
			break ;
		res++;
		while (str[i] != '\0' && str[i] != c)
			i++;
	}
	return (res);
}

static char	*alloc_token(char const *str, char c, int j)
{
	size_t	l;
	size_t	k;
	char	*dst;

	l = len(str, c, j);
	dst = malloc(sizeof(char) * (l + 1));
	if (!dst)
		return (NULL);
	k = 0;
	while (k < l)
	{
		dst[k] = str[j + (int)k];
		k++;
	}
	dst[k] = '\0';
	return (dst);
}

static int	fill(char **res, char const *str, char c, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size && str[j] != '\0')
	{
		if (str[j] == c)
		{
			j++;
			continue ;
		}
		res[i] = alloc_token(str, c, j);
		if (!res[i])
		{
			while (--i >= 0)
				free(res[i]);
			return (0);
		}
		j = j + len(str, c, j);
		i++;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		size;

	if (s == 0)
		return (NULL);
	size = count_word(s, c);
	res = (char **)malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (res);
	if (!fill(res, s, c, size))
	{
		free(res);
		return (NULL);
	}
	res[size] = NULL;
	return (res);
}
