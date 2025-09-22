/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:08 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/12 00:44:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_echo.h"

int	check_big_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	if (str[i] != 'n')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	check_echo_args(t_cmd *cmds)
{
	int	i;
	int	found_n;

	i = 1;
	found_n = 0;
	while (cmds->args[i] && (ft_strcmp(cmds->args[i], "-n") == 0
			|| check_big_n(cmds->args[i]) == 1))
	{
		found_n = 1;
		i++;
	}
	if (found_n)
		return (i);
	else
		return (1);
}

char	*get_env_value(t_list *env, const char *name)
{
	size_t	len;
	t_list	*tmp;

	len = ft_strlen(name);
	tmp = env;
	if (!tmp)
		return (NULL);
	while (tmp)
	{
		if (ft_strncmp(tmp->str, name, len) == 0 && tmp->str[len] == '=')
			return (tmp->str + len + 1);
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (NULL);
}
