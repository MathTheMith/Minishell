/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:08:09 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/29 15:35:25 by mvachon          ###   ########.fr       */
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

int	process_lst(t_cmd **command, int i)
{
	if (command)
		free_all_cmds(*command, 1);
	if (i == 1)
		return (1);
	else
		return (2);
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
}

void process_c(t_cmd **command)
{
    int i;
    
    if (!command)
        return;
    
    i = 0;
    while (command[i]) {
        free_one_cmd(command[i], 1);
        i++;
    }
    free(command);
}

int	process(t_cmd **cmds, t_cmd **command, t_list *env_list, char **envp)
{
	int	save_exit;

	save_exit = (*cmds)->last_exit_code;
	update_command_data(cmds, command, save_exit, env_list);
	if (!*cmds)
		return (0);
	return (check_info(*cmds, command, envp));
}
