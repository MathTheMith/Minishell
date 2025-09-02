/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:51:13 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:42 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*ft_free_all(char **result)
{
	int	i;

	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

static int	ft_makesplit(char const *s, char **result, char c)
{
	int	j;
	int	i;
	int	start;

	j = 0;
	i = 0;
	while (s[j])
	{
		if (s[j] != c)
		{
			start = j;
			while (s[j] && s[j] != c)
				j++;
			result[i] = ft_substr(s, start, j - start);
			if (!result[i++])
			{
				ft_free_all(result);
				return (1);
			}
		}
		else
			j++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char		**result;

	if (!s)
		return (NULL);
	result = (char **)ft_calloc((ft_count_words(s, c) + 1), sizeof(char *));
	if (!result)
		return (NULL);
	if (ft_makesplit(s, result, c) == 0)
		return (result);
	return (NULL);
}
