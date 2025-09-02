/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:40:57 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:38 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	tmp;

	tmp = ft_strlen(dst);
	if (size == 0)
		return (ft_strlen(src));
	if (size <= ft_strlen(dst))
		return (ft_strlen(src) + size);
	i = 0;
	j = 0;
	while (dst[i])
		i++;
	while (i < size - 1 && src[j])
		dst[i++] = src[j++];
	dst[i] = 0;
	return (ft_strlen(src) + tmp);
}
