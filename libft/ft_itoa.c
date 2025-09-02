/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:39:47 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:49 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*dest;
	int		count;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	count = ft_count(n);
	dest = (char *)malloc((count + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	if (n < 0)
	{
		dest[0] = '-';
		n = -n;
	}
	dest[count] = '\0';
	while (n > 0)
	{
		dest[--count] = (n % 10) + '0';
		n /= 10;
	}
	return (dest);
}
