/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:51:38 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:33 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_start(char const *s1, char const *set)
{
	int		i;
	int		j;

	i = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (s1[i] != set[j])
			return (i);
		i++;
	}
	return (0);
}

static int	ft_end(char const *s1, char const *set)
{
	size_t		i;
	size_t		j;

	i = ft_strlen(s1);
	while (i-- != 0)
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (s1[i] != set[j])
			return (i + 1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	char	*dest;
	int		end;
	int		i;

	if (!s1 || !set)
		return (NULL);
	start = ft_start(s1, set);
	end = ft_end(s1, set);
	if (end <= start)
		return (ft_strdup(""));
	dest = ft_calloc(end - start + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (start < end)
	{
		dest[i] = s1[start];
		start++;
		i++;
	}
	return (dest);
}
