/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:31:38 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:31:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_helper(t_cmd *current, int i)
{
	if (current->args)
	{
		while (i < current->nb_args)
		{
			if (current->args[i])
				free(current->args[i]);
			i++;
		}
		free(current->args);
	}
	if (current->quoted)
		free(current->quoted);
	if (current->infile)
		free(current->infile);
	if (current->outfile)
		free(current->outfile);
}

int	handle_parsing_error(t_cmd **command, char *input, t_cmd *cmds)
{
	if (command && *command)
		cmds->last_exit_code = 2;
	else
		cmds->last_exit_code = 2;
	free(input);
	return (0);
}

void	free_all(t_cmd **command, char *input)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	if (input)
		free(input);
	if (!command || !*command)
		return ;
	current = *command;
	while (current)
	{
		i = 0;
		next = current->next;
		free_cmd_helper(current, i);
		current = next;
		free(current);
	}
	*command = NULL;
}

int	process_lst(t_cmd **command, int i)
{
	if (command && *command)  // AJOUT: Vérification
		free_all_cmds(*command, 1);
	if (i == 1)
		return (1);
	else
		return (2);
}

void	cleanup_data(t_data *data_ptr)
{
	if (!data_ptr)
		return ;
	if (data_ptr->input)
		free(data_ptr->input);
	free(data_ptr);
}

int	execute_command(t_cmd **cmds, t_cmd **command,
				t_list *env_list, t_data *data_ptr)
{
	char	**current_envp;
	int		status;

	current_envp = env_list_to_envp(env_list);
	if (current_envp)
		status = process(cmds, command, env_list, current_envp);
	else
	{
		(*cmds)->last_exit_code = process_lst(command, 1);
		status = 0;
	}
	cleanup_data(data_ptr);
	return (status);
}
