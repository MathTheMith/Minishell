/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_and_parse.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:08 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/18 07:35:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

void	free_args(char **args, int last_index)
{
	while (--last_index >= 0)
		free(args[last_index]);
	free(args);
}

char **allocate_args(int count)
{
    char **args = malloc(sizeof(char*) * (count + 1));
    return args;
}

static char	*extract_single_arg(t_cmd *command, char *str, int *pos,
		int arg_quote)
{
	char	*arg;

	arg = extract_and_parse_arg(command, str, pos, arg_quote);
	if (!arg)
		return (NULL);
	return (arg);
}

bool	fill_args(t_cmd *command, char **args, char *str, int arg_count)
{
	int	pos;
	int	i;
	int	arg_quote;

	pos = 0;
	i = 0;
	arg_quote = 0;
	while (i < arg_count)
	{
		args[i] = extract_single_arg(command, str, &pos, arg_quote);
		if (!args[i])
		{
			free_args(args, i);
			return (false);
		}
		i++;
		arg_quote++;
	}
	args[i] = NULL;
	return (true);
}

char	**smart_split_and_parse(t_cmd *command, char *str)
{
	char	**args;
	int		arg_count;

	arg_count = command->nb_args;
	args = allocate_args(arg_count);
	if (!args)
		return (NULL);
	if (!fill_args(command, args, str, arg_count))
		return (NULL);
	return (args);
}
