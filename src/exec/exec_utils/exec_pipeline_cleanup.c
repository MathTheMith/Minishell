/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_cleanup.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:23:59 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:23:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipeline.h"
#include "exec_cmd_utils.h"

static void	free_child_resources(t_cmd *cmds, char **envp, char **cleaned_args)
{
	if (cleaned_args)
		free_cleaned_args(cleaned_args);
	if (envp)
		free_string_array(envp);
	if (cmds)
	{
		if (cmds->env)
			free_env_list(cmds->env);
		if (cmds->pids)
			free(cmds->pids);
		if (cmds->command_array)
			free(cmds->command_array);
	}
}

static void	free_cmd_chain(t_cmd *cmds)
{
	t_cmd	*temp;
	t_cmd	*next;

	temp = cmds;
	while (temp)
	{
		next = temp->next;
		free_one_cmd(temp, 1);
		temp = next;
	}
}

static void	free_data_structure(t_data *data_to_free)
{
	if (!data_to_free)
		return ;
	if (data_to_free->input)
		free(data_to_free->input);
	free(data_to_free);
}

void	cleanup_and_exit_child(t_cmd *cmds, char **envp,
				char **cleaned_args, int exit_code)
{
	t_data	*data_to_free;

	data_to_free = NULL;
	if (cmds && cmds->data)
		data_to_free = cmds->data;
	free_child_resources(cmds, envp, cleaned_args);
	if (cmds)
		free_cmd_chain(cmds);
	free_data_structure(data_to_free);
	rl_clear_history();
	exit(exit_code);
}
