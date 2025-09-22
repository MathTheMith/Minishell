/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:23:29 by marvin            #+#    #+#             */
/*   Updated: 2025/09/15 18:23:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoll_safe_finish(int sign, unsigned long long result, long long *out)
{
	if (sign == -1)
		*out = -(long long)result;
	else
		*out = (long long)result;
	return (1);
}

int	ft_atoll_safe(const char *str, long long *out)
{
	unsigned long long	result;
	int					sign;
	int					i;
	int					digit;

	result = 0;
	*out = 0;
	sign = 0;
	i = 0;
	if (!ft_is_valid_number(str, &sign, &i))
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (sign == 1)
		{
			if (result > (unsigned long long)(LLONG_MAX - digit) / 10)
				return (0);
		}
		else if (result > ((unsigned long long)LLONG_MAX + 1 - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	return (ft_atoll_safe_finish(sign, result, out));
}
