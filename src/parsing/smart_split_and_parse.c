/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_and_parse.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:41:36 by tomm              #+#    #+#             */
/*   Updated: 2025/08/26 15:38:35 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Libère un tableau de chaînes jusqu'à l'indice donné
void	free_args(char **args, int last_index)
{
	while (--last_index >= 0)
		free(args[last_index]);
	free(args);
}

// Alloue et initialise le tableau des arguments
char	**allocate_args(int count)
{
	char **args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	return args;
}

// Remplit le tableau args avec les arguments extraits
bool	fill_args(t_cmd *command, char **args, char *str, int arg_count)
{
	int	pos = 0;
	int	i = 0;
	int	arg_quote = 0;

	while (i < arg_count)
	{
		args[i] = extract_and_parse_arg(command, str, &pos, arg_quote);
		if (!args[i])
		{
			free_args(args, i);
			return false;
		}
		i++;
		arg_quote++;
	}
	args[i] = NULL;
	return true;
}

// Fonction principale
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
	return args;
}
