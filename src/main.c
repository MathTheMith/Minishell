/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:22 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/30 16:00:13 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*init_cmds(void)
{
	t_cmd	*cmds;

	cmds = malloc(sizeof(t_cmd));
	if (!cmds)
		return (NULL);
	ft_memset(cmds, 0, sizeof(t_cmd));
	cmds->last_exit_code = 0;
	return (cmds);
}

void	update_command_data(t_cmd **cmds, t_cmd **command, int save_exit,
		t_list *env_list)
{
	if (!command || !*command)
	{
		(*cmds)->last_exit_code = save_exit;
		return ;
	}
	if (*cmds && *cmds != *command)
		free_all_cmds(*cmds, 1);
	*cmds = *command;
	*command = NULL;
	(*cmds)->last_exit_code = save_exit;
	(*cmds)->env = env_list;
}

void	free_env_list(t_list *env_list)
{
	t_list	*current;
	t_list	*next;

	if (!env_list)
		return ;
	current = env_list;
	while (current)
	{
		next = current->next;
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		current->prev = NULL;
		current->next = NULL;
		free(current);
		if (next == env_list || next == NULL)
			break ;
		current = next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	*cmds;
	t_list	*env_list;
	int		status;

	(void)ac;
	(void)av;
	cmds = init_cmds();
	if (!cmds)
		return (1);
	env_list = create_env_list(envp);
	setup_parent_signals();
	status = process_input_loop(cmds, env_list);
	rl_clear_history();
	free_env_list(env_list);
	return (status);
}
