/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_invalid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:40 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:41 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"

static int	check_more(char *input, int *i, int *in_s, int *in_d)
{
	if (input[*i] == '\'' && !*in_d)
		*in_s = !*in_s;
	else if (input[*i] == '"' && !*in_s)
		*in_d = !*in_d;
	else if (!*in_s && !*in_d)
	{
		if (check_invalid_middle(input, *i) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_invalid(char *input)
{
	int	i;
	int	in_single;
	int	in_double;

	if (!input || !*input)
		return (EXIT_FAILURE);
	i = 0;
	while ((input[i] == ' ' || input[i] == '\t') && input[i])
		i++;
	if (check_invalid_start(input, i) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		if (check_more(input, &i, &in_single, &in_double) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	if (in_single || in_double)
	{
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
