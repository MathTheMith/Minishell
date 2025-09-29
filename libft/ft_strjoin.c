/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:47:06 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/29 15:22:53 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*dest;
	int		len;
	int		j;

	i = ft_strlen(s1);
	len = (ft_strlen(s1) + ft_strlen(s2));
	dest = malloc((len + 1) * sizeof(char));
	if (dest == 0)
		return (NULL);
	dest = ft_memcpy(dest, s1, ft_strlen(s1));
	j = 0;
	while (i < len)
	{
		dest[i] = s2[j];
		i++;
		j++;
	}
	dest[i] = 0;
	return (dest);
}
