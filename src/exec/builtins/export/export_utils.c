/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:53 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:54 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

bool	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	exist(char *str, t_list *env)
{
	int		i;
	int		len;
	t_list	*tmp;

	i = 0;
	len = 0;
	tmp = env;
	if (!env)
		return (-1);
	while (str[len] && str[len] != '=' && !(str[len] == '+' && str[len
				+ 1] == '='))
		len++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->str, str, len) && (tmp->str[len] == '\0'
				|| tmp->str[len] == '='))
			return (i);
		tmp = tmp->next;
		i++;
		if (tmp == env)
			break ;
	}
	return (-1);
}

char	*get_var_value(char *var_name, t_list *env)
{
	t_list	*tmp;
	int		name_len;

	tmp = env;
	name_len = ft_strlen(var_name);
	if (!env)
		return (NULL);
	while (tmp)
	{
		if (!ft_strncmp(tmp->str, var_name, name_len)
			&& tmp->str[name_len] == '=')
			return (&tmp->str[name_len + 1]);
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (NULL);
}

bool	is_append_operation(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (true);
		if (str[i] == '=')
			return (false);
		i++;
	}
	return (false);
}
