/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:03 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:04 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cd.h"
#include "ft_echo.h"

char	*expand_tilde(char *dir, t_cmd *cmds)
{
	char	*home;

	if (dir[0] != '~')
		return (dir);
	home = get_env_value(cmds->env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		cmds->last_exit_code = 1;
		return (NULL);
	}
	if (dir[1] == '\0')
		return (home);
	else if (dir[1] == '/')
		return (ft_strjoin(home, dir + 1));
	return (dir);
}

char	*resolve_env_var(char *dir, t_cmd *cmds)
{
	char	*resolved_dir;

	if (!dir || dir[0] != '$')
		return (dir);
	resolved_dir = get_env_value(cmds->env, dir + 1);
	if (resolved_dir)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		cmds->last_exit_code = 1;
		return (NULL);
	}
	return (dir);
}

static t_list	*find_env_var(t_list *env, char *name)
{
	t_list	*tmp;
	size_t	name_len;

	name_len = ft_strlen(name);
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->str, name, name_len) == 0
			&& tmp->str[name_len] == '=')
			return (tmp);
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (NULL);
}

static t_list	*create_env_node(char *name, char *value)
{
	t_list	*new_node;
	char	*new_var;
	size_t	name_len;
	size_t	value_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
		return (NULL);
	ft_strcpy(new_var, name);
	new_var[name_len] = '=';
	ft_strcpy(new_var + name_len + 1, value);
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(new_var);
		return (NULL);
	}
	new_node->str = new_var;
	return (new_node);
}

void	add_to_env(t_list **env, char *name, char *value)
{
	t_list	*existing;
	t_list	*new_node;

	existing = find_env_var(*env, name);
	if (existing)
	{
		free(existing->str);
		new_node = create_env_node(name, value);
		if (new_node)
			existing->str = new_node->str;
		free(new_node);
		return ;
	}
	new_node = create_env_node(name, value);
	if (!new_node)
		return ;
	new_node->next = *env;
	new_node->prev = (*env)->prev;
	(*env)->prev->next = new_node;
	(*env)->prev = new_node;
}
