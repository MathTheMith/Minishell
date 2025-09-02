/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 12:42:30 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:47 by tfournie         ###   ########.fr       */
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
