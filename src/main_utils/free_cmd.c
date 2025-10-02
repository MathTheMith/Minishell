/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:15:37 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:57:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "path_exec.h"

void	free_cmd(t_cmd_exec *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_paths(cmd->args);
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	free(cmd);
	cmd = NULL;
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	free(data);
	data = NULL;
}

static void	free_cmd_fields(t_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		free_args_array(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->quoted)
	{
		free(cmd->quoted);
		cmd->quoted = NULL;
	}
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->heredoc_buffer)
	{
		i = 0;
		while (i < cmd->heredoc_count)
		{
			if (cmd->heredoc_buffer[i])
				free(cmd->heredoc_buffer[i]);
			i++;
		}
		free(cmd->heredoc_buffer);
		cmd->heredoc_buffer = NULL;
	}
}

void	free_one_cmd(t_cmd *cmd, int is_free)
{
	if (!cmd)
		return ;
	free_cmd_fields(cmd);
	if (is_free)
	{
		free(cmd);
		cmd = NULL;
	}
}

void	free_all_cmds(t_cmd *cmds, int is_free)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		free_one_cmd(cmds, is_free);
		cmds = next;
	}
}
