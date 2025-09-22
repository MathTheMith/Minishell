/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:57 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:58 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"
#include "ft_echo.h"

int	handle_exit_code(t_cmd *command, t_quote_params *params)
{
	(*(params->j))++;
	copy_exit_code_to_out(command->data->lec_save, params->out, params->k);
	return (0);
}

int	handle_env_expansion(t_cmd *command, t_quote_params *params)
{
	char	env_name[MAX_ENV_NAME];
	char	*env_val;
	int		name_len;

	name_len = extract_env_name(params->str, params->j, env_name);
	if (name_len == 0)
	{
		params->out[(*(params->k))++] = '$';
		return (0);
	}
	env_val = get_env_value(command->env, env_name);
	copy_env_value_to_out(env_val, params->out, params->k);
	return (0);
}
