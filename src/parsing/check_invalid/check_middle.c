/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_middle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:43 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:44 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"

static int	check_invalid_pipe_usage(char *input, int i)
{
	int	j;

	if (input[i] == '|' && input[i + 1] != '|')
	{
		j = i + 1;
		while (input[j] && input[j] == ' ')
			j++;
		if (!input[j])
			return (print_syntax_error("newline"));
		if (input[j] == '|')
			return (print_syntax_error("|"));
	}
	return (EXIT_SUCCESS);
}

int	check_invalid_middle(char *input, int i)
{
	if (input[i] == '&' && input[i + 1] == '&')
		return (print_syntax_error("&&"));
	if (input[i] == '|' && input[i + 1] == '|')
		return (print_syntax_error("||"));
	if (check_invalid_pipe_usage(input, i) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (input[i] == '\\')
		return (print_syntax_error("\\"));
	if (input[i] == ';')
		return (print_syntax_error(";"));
	if (input[i] == '(')
		return (print_syntax_error("("));
	if (input[i] == ')')
		return (print_syntax_error(")"));
	if (input[i] == '&' && input[i + 1] != '&')
		return (print_syntax_error("&"));
	return (EXIT_SUCCESS);
}
