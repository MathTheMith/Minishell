/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:31:05 by tfournie          #+#    #+#             */
/*   Updated: 2025/02/27 19:36:07 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_skip_whitespace_and_sign(const char *nptr, int *sign)
{
	int	i;

	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			*sign = -1;
		else
			*sign = 1;
		i++;
	}
	return (i);
}

long	ft_atoi(const char *nptr)
{
	int			i;
	int			sign;
	long long	res;

	sign = 1;
	i = ft_skip_whitespace_and_sign(nptr, &sign);
	res = 0;
	if (!ft_isdigit(nptr[i]))
		return ((long)INT_MAX + 1);
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		res = res * 10 + (nptr[i] - '0');
		if (res * sign > INT_MAX)
			return ((long)INT_MAX + 1);
		if (res * sign < INT_MIN)
			return ((long)INT_MIN - 1);
		i++;
	}
	if (nptr[i] != '\0')
		return ((long)INT_MAX + 1);
	return ((int)(res * sign));
}
