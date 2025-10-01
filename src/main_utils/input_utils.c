/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:09:43 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/30 16:25:56 by mvachon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_elements(t_list *env_list)
{
	t_list	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
		if (current == env_list)
			break ;
	}
	return (count);
}

static void	cleanup_envp(char **envp, int up_to_index)
{
	int	i;

	i = 0;
	while (i < up_to_index)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	envp = NULL;
}

static int	fill_envp_array(char **envp, t_list *env_list, int count)
{
	t_list	*current;
	int		i;

	i = 0;
	current = env_list;
	while (i < count)
	{
		envp[i] = ft_strdup(current->str);
		if (!envp[i])
		{
			cleanup_envp(envp, i);
			return (0);
		}
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (1);
}

char	**env_list_to_envp(t_list *env_list)
{
	char	**envp;
	int		count;

	if (!env_list)
		return (NULL);
	count = count_env_elements(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (!fill_envp_array(envp, env_list, count))
		return (NULL);
	return (envp);
}
