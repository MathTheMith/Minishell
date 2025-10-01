/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:13:20 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:17:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "check_invalid.h"
#include "exec_process.h"

static int	validate_commands(t_cmd *cmds, t_cmd **command)
{
	t_cmd	*cur;

	if (cmds)
		cmds->command_array = command;
	cur = cmds;
	while (cur)
	{
		cur->command_array = command;
		if (cur->args && cur->args[0])
		{
			if (check_invalid_start(cur->args[0], 0) == EXIT_FAILURE)
			{
				free_all_cmds(cmds, 1);
				return (-1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int	check_info(t_cmd *cmds, t_cmd **command, char **envp)
{
	if (validate_commands(cmds, command) == -1)
		return (-1);
	if (cmds && cmds->next)
		exec_pipeline(cmds, envp);
	else
		check_args(cmds, envp);
	free_envp(envp);
	envp = NULL;
	return (1);
}
