/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 12:42:30 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 17:28:22 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*col;
	const unsigned char	*cpy;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	col = (unsigned char *) dest;
	cpy = (const unsigned char *) src;
	i = 0;
	while (i < n)
	{
		col[i] = cpy[i];
		i++;
	}
	return (dest);
}
