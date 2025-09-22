/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:38 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:39 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

static void	free_str_array(char **arr, int count)
{
	while (--count >= 0)
		free(arr[count]);
	free(arr);
}

static bool	fill_array_from_list(t_list *env, char **arr)
{
	t_list	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->str);
		if (!arr[i])
		{
			free_str_array(arr, i);
			return (false);
		}
		i++;
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	arr[i] = NULL;
	return (true);
}

char	**lst_to_arr(t_list *env)
{
	char	**arr;
	int		len;

	if (!env)
		return (NULL);
	len = lst_size(env);
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	if (!fill_array_from_list(env, arr))
		return (NULL);
	return (arr);
}
