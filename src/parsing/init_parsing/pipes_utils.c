/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:39 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:40 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

bool	check_pipe_in(char **cmd_pipe, int i)
{
	if (i > 0 && cmd_pipe[i - 1] != NULL)
		return (true);
	return (false);
}

bool	check_pipe_out(char **cmd_pipe, int i)
{
	if (cmd_pipe[i + 1] != NULL)
		return (true);
	return (false);
}

void	update_quote_state_pipe(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

int	count_pipes(char *input)
{
	int	pipe_count;
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	pipe_count = 0;
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		update_quote_state_pipe(input[i], &in_single_quote, &in_double_quote);
		if (input[i] == '|' && !in_single_quote && !in_double_quote)
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

void	free_result_array(char **result, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(result[j]);
		j++;
	}
	free(result);
}
