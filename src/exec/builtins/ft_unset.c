/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:22:52 by marvin            #+#    #+#             */
/*   Updated: 2025/09/11 22:22:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_name_len(char *name)
{
	int	len;

	len = 0;
	while (name && name[len])
		len++;
	return (len);
}

static void	delete_node(t_list **env, t_list *node)
{
	if (node->next == node && node->prev == node)
	{
		free(node->str);
		free(node);
		*env = NULL;
		return ;
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	if (*env == node)
		*env = node->next;
	free(node->str);
	free(node);
}

bool	remove_env_var(char *name, t_list **env)
{
	t_list	*tmp;
	int		name_len;

	if (!*env || !name)
		return (false);
	name_len = get_name_len(name);
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->str, name, name_len) && (tmp->str[name_len] == '\0'
				|| tmp->str[name_len] == '='))
		{
			delete_node(env, tmp);
			return (true);
		}
		tmp = tmp->next;
		if (tmp == *env)
			break ;
	}
	return (false);
}

void	unset_input(t_cmd *cmds, t_list **env)
{
	int	i;

	i = 1;
	if (ft_strcmp(cmds->args[0], "unset") != 0)
		return ;
	if (!cmds->args[1])
	{
		cmds->last_exit_code = 0;
		return ;
	}
	while (cmds->args[i])
	{
		remove_env_var(cmds->args[i], env);
		i++;
	}
	cmds->last_exit_code = 0;
}
