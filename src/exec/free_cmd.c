/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:36:28 by marvin            #+#    #+#             */
/*   Updated: 2025/05/28 00:36:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd_exec *cmd)
{
	if (!cmd)
		return;
	if (cmd->raw_cmd)
		free(cmd->raw_cmd);
	if (cmd->args)
		free_paths(cmd->args);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}


void	free_all_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd)
			free(cmd);
		cmd = tmp;
	}
}
