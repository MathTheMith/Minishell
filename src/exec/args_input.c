/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:14:48 by mvachon           #+#    #+#             */
/*   Updated: 2025/09/07 11:08:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_vars(t_cmd *cmds)
{
	int		i;
	char	*env_val;

	i = 0;
	while (cmds->args[i])
	{
		if (cmds->args[i][0] == '$')
		{
			env_val = getenv(cmds->args[i] + 1);
			if (env_val)
			{
				free(cmds->args[i]);
				cmds->args[i] = strdup(env_val);
			}
			else
			{
				free(cmds->args[i]);
				cmds->args[i] = strdup("");
			}
		}
		i++;
	}
}
