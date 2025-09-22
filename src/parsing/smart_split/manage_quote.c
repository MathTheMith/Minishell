/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:05 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:05:06 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

int	handle_single_quotes(t_quote_params *params)
{
	(*(params->j))++;
	while (params->str[*(params->j)] && params->str[*(params->j)] != '\'')
	{
		params->out[(*(params->k))++] = params->str[(*(params->j))++];
	}
	if (params->str[*(params->j)] == '\'')
		(*(params->j))++;
	return (0);
}

int	handle_env_variable(t_cmd *command, t_quote_params *params)
{
	(*(params->j))++;
	if (params->str[*(params->j)] == '?')
		return (handle_exit_code(command, params));
	else
		return (handle_env_expansion(command, params));
}

static void	process_double_quote_char(t_cmd *command, t_quote_params *params)
{
	if (params->str[*(params->j)] == '$')
		handle_env_variable(command, params);
	else
		params->out[(*(params->k))++] = params->str[(*(params->j))++];
}

int	handle_double_quotes(t_cmd *command, t_quote_params *params)
{
	(*(params->j))++;
	while (params->str[*(params->j)] && params->str[*(params->j)] != '\"')
	{
		process_double_quote_char(command, params);
	}
	if (params->str[*(params->j)] == '\"')
		(*(params->j))++;
	return (0);
}
