/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_numeric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:33:48 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 23:33:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_numeric(const char *str)
{
    int i = 0;

    if (!str || str[0] == '\0')
        return false;

    if (str[0] == '+' || str[0] == '-')
        i++;

    if (str[i] == '\0')
        return false;

    while (str[i])
    {
        if (!ft_isdigit((unsigned char)str[i]))
            return false;
        i++;
    }
    return true;
}
