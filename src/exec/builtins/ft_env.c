/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:13 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 17:02:02 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "path_exec.h"

static int	validate_env_command(t_cmd *cmds, char **envp)
{
	if (!cmds || !cmds->args || !cmds->args[0]
		|| ft_strcmp(cmds->args[0], "env") != 0)
		return (0);
	if (!cmds->env)
		return (0);
	if (get_path_env(envp) == NULL)
	{
		ft_putstr_fd("env: No such file or directory\n", 2);
		cmds->last_exit_code = 127;
		return (0);
	}
	if (cmds->args[1] && !cmds->pipe_out)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		cmds->last_exit_code = 1;
		return (0);
	}
	return (1);
}

void	env_input(t_cmd *cmds, char **envp)
{
	t_list	*temp;

	if (!validate_env_command(cmds, envp))
		return ;
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
			break ;
	}
}

static t_list	*env_list_new_node(const char *env_str)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->str = ft_strdup(env_str);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

static void	env_list_append(t_list **head, t_list **tail, t_list *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		new_node->prev = *tail;
		*tail = new_node;
	}
}

t_list	*create_env_list(char **envp)
{
	t_list	*head;
	t_list	*tail;
	int		i;
	t_list	*new_node;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = env_list_new_node(envp[i]);
		if (!new_node)
			return (NULL);
		env_list_append(&head, &tail, new_node);
		i++;
	}
	if (head)
	{
		head->prev = tail;
		tail->next = head;
	}
	return (head);
}
