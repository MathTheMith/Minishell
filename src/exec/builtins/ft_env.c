/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:22 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_input(t_cmd *cmds)
{
    t_list *temp;

    if (!cmds || !cmds->args || !cmds->args[0] || ft_strcmp(cmds->args[0], "env") != 0)
        return;
    if (!cmds->env)
	{
        return;
	}
    if (cmds->args[1] && !cmds->pipe_out)
    {
        ft_putstr_fd("env: too many arguments\n", 2);
        cmds->last_exit_code = 1;
        return;
    }
    temp = cmds->env;
    while (1)
    {
        if (ft_strchr(temp->str, '='))
		{
            ft_putstr_fd(temp->str, 1);
			ft_putstr_fd("\n", 1);
		}
        temp = temp->next;
        if (temp == cmds->env)
            break;
    }
    cmds->last_exit_code = 0;
}


t_list *create_env_list(char **envp)
{
	t_list *head = NULL;
	t_list *new_node;
	t_list *tail = NULL;
	int i = 0;

	while (envp[i])
	{
		new_node = malloc(sizeof(t_list));
		if (!new_node)
			return NULL;

		new_node->str = strdup(envp[i]);
		new_node->next = NULL;
		new_node->prev = NULL;

		if (!head)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			new_node->prev = tail;
			tail = new_node;
		}
		i++;
	}
	if (head)
	{
		head->prev = tail;
		tail->next = head;
	}
	return head;
}
