/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:14:02 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:46 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*cpy;
	size_t			i;

	if (!dest && !src)
		return (NULL);
	cpy = (unsigned char *) src;
	i = 0;
	if (dest > src)
	{
		while (--n + 1 > 0)
			((unsigned char *)dest)[n] = cpy[n];
	}
	else
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = cpy[i];
			i++;
		}
	}
	return (dest);
}
