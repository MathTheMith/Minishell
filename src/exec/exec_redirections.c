/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd_utils.h"

static int	count_clean_args(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0)
			&& args[i + 1])
			i += 2;
		else if (args[i][0] == '\0')
			i++;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static int	process_arg(char **args, char **new_args, int *i, int *j)
{
	if ((ft_strcmp(args[*i], ">") == 0 || ft_strcmp(args[*i], ">>") == 0
			|| ft_strcmp(args[*i], "<") == 0
			|| ft_strcmp(args[*i], "<<") == 0)
		&& args[*i + 1])
		*i += 2;
	else if (args[*i][0] == '\0')
		(*i)++;
	else
	{
		new_args[*j] = ft_strdup(args[*i]);
		if (!new_args[*j])
		{
			free_clean_args(new_args, *j);
			return (0);
		}
		(*j)++;
		(*i)++;
	}
	return (1);
}

static int	fill_clean_args(char **args, char **new_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (!process_arg(args, new_args, &i, &j))
			return (0);
	}
	new_args[j] = NULL;
	return (1);
}

char	**remove_redirections(char **args)
{
	int		count;
	char	**new_args;

	count = count_clean_args(args);
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	if (!fill_clean_args(args, new_args))
		return (NULL);
	return (new_args);
}

int	count_args_without_redirections(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], ">") == 0
				|| ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<<") == 0)
			&& args[i + 1])
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}
