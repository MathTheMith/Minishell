/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:22:36 by marvin            #+#    #+#             */
/*   Updated: 2025/09/29 14:37:21 by mvachon          ###   ########.fr       */
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
}
void	free_one_cmd(t_cmd *cmd, int is_free)
{
	int	i;

	if (!cmd)
		return ;
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
	if (cmd->quoted)
		free(cmd->quoted);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (is_free)
		free(cmd);
}

void	free_all_cmds(t_cmd *cmds, int is_free)
{
	t_cmd	*next;
	t_data	*data_to_free;

	data_to_free = NULL;

	if (cmds && cmds->data)
		data_to_free = cmds->data;
	while (cmds)
	{
		next = cmds->next;
		free_one_cmd(cmds, is_free);
		cmds = next;
	}
	if (data_to_free)
	{
		free_data(data_to_free);
	}
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
