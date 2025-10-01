/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:22 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/01 16:05:29 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"
#include "redirections_parsing.h"

static int	allocate_cmd_structure(t_cmd **command, int i)
{
	command[i] = malloc(sizeof(t_cmd));
	if (command[i] == NULL)
		return (0);
	ft_memset(command[i], 0, sizeof(t_cmd));
	command[i]->pids = NULL;
	command[i]->cmd_count = 0;
	return (1);
}

static void	setup_basic_fields(t_cmd *cmd, t_cmd_init_params *params)
{
	cmd->data = params->data;
	cmd->id = params->i;
	cmd->env = params->env_list;
	cmd->next = NULL;
}

static int	set(t_cmd *cmd, char *processed_cmd, char *original_cmd)
{
	cmd->nb_args = count_args(processed_cmd);
	cmd->quoted = malloc(sizeof(int) * cmd->nb_args);
	if (!cmd->quoted)
		return (0);
	ft_memset(cmd->quoted, 0, sizeof(int) * cmd->nb_args);
	cmd->args = manage_args(cmd, original_cmd);
	//check
	return (1);
}

static void	setup_redirections(t_cmd *cmd, char *processed_cmd, char **cmd_pipe,
		int i)
{
	cmd->infile = check_infile(processed_cmd);
	cmd->ascending = check_ascending(processed_cmd);
	cmd->outfile = check_outfile(processed_cmd);
	cmd->descending = check_descending(processed_cmd);
	cmd->pipe_in = check_pipe_in(cmd_pipe, i);
	cmd->pipe_out = check_pipe_out(cmd_pipe, i);
}

void	init_cmd(t_cmd **command, t_cmd_init_params *params)
{
	char	*processed_cmd;

	processed_cmd = add_spaces_around_redirections(params->cmd_pipe[params->i]);
	if (!processed_cmd)
	{
		command[params->i] = NULL;
		return ;
	}
	if (!allocate_cmd_structure(command, params->i))
	{
		free(processed_cmd);
		free(command);
		return ;
	}
	setup_basic_fields(command[params->i], params);//retour
	if (!set(command[params->i], processed_cmd, params->cmd_pipe[params->i]))
	{
		free(processed_cmd);
		free(command[params->i]);
		command[params->i] = NULL;
		return ;
	}
	setup_redirections(command[params->i], processed_cmd, params->cmd_pipe,
		params->i);
	free(processed_cmd);
}
