/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:16:24 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 01:20:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	process_a(char *input)
{
	if (!input)
	{
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	return (0);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	envp = NULL;
}

void	process_c(t_cmd **command)
{
	int	i;

	if (!command)
		return ;
	i = 0;
	while (command[i])
	{
		free_one_cmd(command[i], 1);
		i++;
	}
	free(command);
	command = NULL;
}

int	process(t_cmd **cmds, t_cmd **command, t_list *env_list, char **envp)
{
	int	save_exit;

	save_exit = (*cmds)->last_exit_code;
	update_command_data(cmds, command, save_exit, env_list);
	if (!*cmds)
		return (0);
	process_c(command);
	return (check_info(*cmds, NULL, envp));
}
