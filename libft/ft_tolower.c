/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:03:56 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:32 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int character)
{
	if (character == -1)
		return (-1);
	if (character >= 'A' && character <= 'Z')
		return ((unsigned char)character + 32);
	return ((unsigned char)character);
}
