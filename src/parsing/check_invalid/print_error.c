/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:48 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:49 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"

int	print_syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)token, 2);
	ft_putstr_fd("'\n", 2);
	return (EXIT_FAILURE);
}
