/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_invalid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 22:57:22 by marvin            #+#    #+#             */
/*   Updated: 2025/08/27 23:00:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)token, 2);
	ft_putstr_fd("'\n", 2);
	return (EXIT_FAILURE);
}

int	check_start(char *input, int i)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			return (syntax_error("||"));
		return (syntax_error("|"));
	}
	if (input[i] == '&')
	{
		if (input[i + 1] == '&')
			return (syntax_error("&&"));
		return (syntax_error("&"));
	}
	if (input[i] == ';')
		return (syntax_error(";"));
	if (input[i] == '(')
		return (syntax_error("("));
	if (input[i] == ')')
		return (syntax_error(")"));
	if (input[i] == '.' && (!input[i + 1]))
	    return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}


int	check_middle(char *input, int i)
{
	int	j;

	if (input[i] == '&' && input[i + 1] == '&')
		return (syntax_error("&&"));

	if (input[i] == '|' && input[i + 1] == '|')
		return (syntax_error("||"));

	if (input[i] == '|' && input[i + 1] != '|')
	{
		j = i + 1;
		while (input[j] && input[j] == ' ')
			j++;
		if (!input[j])
			return (syntax_error("newline"));
		if (input[j] == '|')
			return (syntax_error("|"));
	}

	if (input[i] == '\\')
		return (syntax_error("\\"));
	if (input[i] == ';')
		return (syntax_error(";"));
	if (input[i] == '(')
		return (syntax_error("("));
	if (input[i] == ')')
		return (syntax_error(")"));
	if (input[i] == '&' && input[i + 1] != '&')
		return (syntax_error("&"));

	return (EXIT_SUCCESS);
}

int	check_invalid(char *input)
{
	int	i = 0;
	int	in_single = 0;
	int	in_double = 0;

	if (!input || !*input)
		return (EXIT_FAILURE);

	while ((input[i] == ' ' || input[i] == '\t') && input[i])
		i++;

	if (check_start(input, i) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double)
		{
			if (check_middle(input, i) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		i++;
	}

	if (in_single || in_double)
	{
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
