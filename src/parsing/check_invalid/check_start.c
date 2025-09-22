/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_start.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:45 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:46 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"

static int	check_invalid_start_pipe_amp(char *input, int i)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			return (print_syntax_error("||"));
		else
			return (print_syntax_error("|"));
	}
	if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			return (print_syntax_error("&&"));
		else
			return (print_syntax_error("&"));
	}
	return (EXIT_SUCCESS);
}

int	check_invalid_start(char *input, int i)
{
	if (input[i] == '|' || input[i] == '&')
		return (check_invalid_start_pipe_amp(input, i));
	if (input[i] == ';')
		return (print_syntax_error(";"));
	if (input[i] == '(')
		return (print_syntax_error("("));
	if (input[i] == ')')
		return (print_syntax_error(")"));
	if (input[i] == '.' && (!input[i + 1]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
