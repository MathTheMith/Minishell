/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:42 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:43 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

int	lst_size(t_list *env)
{
	int		len;
	t_list	*tmp;

	len = 0;
	tmp = env;
	if (!env)
		return (0);
	while (tmp)
	{
		len++;
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (len);
}

int	len_list(t_list *env)
{
	int		len;
	t_list	*tmp;

	len = 0;
	if (!env)
		return (0);
	tmp = env;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (len);
}

bool	append(t_list **env, char *value)
{
	t_list	*new_node;
	t_list	*last;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (false);
	new_node->str = value;
	if (!(*env))
	{
		new_node->next = new_node;
		new_node->prev = new_node;
		*env = new_node;
		return (true);
	}
	last = (*env)->prev;
	last->next = new_node;
	new_node->prev = last;
	new_node->next = *env;
	(*env)->prev = new_node;
	return (true);
}
