/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:04 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/15 20:38:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"
#include "redirections_parsing.h"

static char	**alloc_cmd_and_get_cmd_pipes(t_cmd ***command, t_data *data)
{
	char	**pipes;

	data->nb_cmd = count_cmd(data);
	*command = malloc((data->nb_cmd + 1) * sizeof(t_cmd *));
	if (*command == NULL)
		return (NULL);
	ft_memset(*command, 0, (data->nb_cmd + 1) * sizeof(t_cmd *));
	if (data->input[0] == '|')
	{
		free(*command);
		return (NULL);
	}
	pipes = split_by_pipes(data->input);
	if (!pipes)
	{
		free(*command);
		return (NULL);
	}
	return (pipes);
}

static int	init_commands_loop(t_cmd **command, t_data *data, char **cmd_pipe,
		t_list *env_list)
{
	int					i;
	t_cmd_init_params	params;

	i = 0;
	params.data = data;
	params.cmd_pipe = cmd_pipe;
	params.env_list = env_list;
	while (i < data->nb_cmd)
	{
		params.i = i;
		init_cmd(command, &params);
		if (command[i] == NULL)
		{
			free_partial_commands(command, i);
			return (i);
		}
		i++;
	}
	command[data->nb_cmd] = NULL;
	return (-1);
}

static void	chain_and_free_pipes(t_cmd **command, t_data *data, char **cmd_pipe)
{
	int	i;

	if (!command || !data || !cmd_pipe)
		return ;
	i = 0;
	while (i < data->nb_cmd - 1)
	{
		if (command[i] && command[i]->pipe_out == true && command[i + 1])
			command[i]->next = command[i + 1];
		i++;
	}
	i = 0;
	while (cmd_pipe[i])
	{
		free(cmd_pipe[i]);
		i++;
	}
	free(cmd_pipe);
}

void	init_command(t_cmd ***command, t_data *data, t_list *env_list)
{
	char	**cmd_pipe;
	int		failed_idx;

	cmd_pipe = alloc_cmd_and_get_cmd_pipes(command, data);
	if (!cmd_pipe)
		return ;
	failed_idx = init_commands_loop(*command, data, cmd_pipe, env_list);
	if (failed_idx != -1)
	{
		*command = NULL;
		free_failed_init(*command, failed_idx, cmd_pipe);
		return ;
	}
	chain_and_free_pipes(*command, data, cmd_pipe);
}
