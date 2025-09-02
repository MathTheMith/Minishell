/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 13:34:49 by tfournie          #+#    #+#             */
/*   Updated: 2024/12/04 17:30:31 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int character)
{
	if (character == -1)
		return (-1);
	if (character >= 'a' && character <= 'z')
		return ((unsigned char)character - 32);
	return ((unsigned char)character);
}
