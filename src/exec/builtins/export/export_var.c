/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:56 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:57 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

static void	update_existing_var(t_list **env, int pos, char *new_value)
{
	int	i;

	i = 0;
	while (i < pos)
	{
		*env = (*env)->next;
		i++;
	}
	free((*env)->str);
	(*env)->str = new_value;
}

static bool	add_new_var(t_list **env, char *new_value)
{
	if (!append(env, new_value))
	{
		free(new_value);
		return (false);
	}
	return (true);
}

bool	export_var(char *str, t_list **env)
{
	int		pos;
	char	*new_value;

	pos = exist(str, *env);
	if (is_append_operation(str))
		new_value = ft_strdup(str);
	else
		new_value = ft_strdup(str);
	if (!new_value)
		return (false);
	if (pos >= 0)
		update_existing_var(env, pos, new_value);
	else if (!add_new_var(env, new_value))
		return (false);
	return (true);
}

char	*extract_var_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str, i + 1);
	return (name);
}

char	*extract_var_value(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		return (ft_strdup(&str[i + 1]));
	return (NULL);
}
