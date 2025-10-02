/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:37 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 01:09:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"
#include "smart_split.h"

char	**manage_args(t_cmd *command, char *input_str)
{
	char	*str_dup;
	char	*spaced;
	char	**args;

	str_dup = ft_strdup(input_str);
	if (!str_dup)
		return (NULL);
	spaced = add_spaces_around_redirections(str_dup);
	free(str_dup);
	if (!spaced)
		return (NULL);
	args = smart_split_and_parse(command, spaced);
	free(spaced);
	if (!args)
	{
		args = malloc(sizeof(char *) * 1);
		if (args)
			args[0] = NULL;
	}
	return (args);
}
