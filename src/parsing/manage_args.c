/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:20:55 by tomm              #+#    #+#             */
/*   Updated: 2025/09/02 14:06:30 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**manage_args(t_cmd *command, char *input_str)
{
	char	*str_dup;
	char	**args;
	
	str_dup = ft_strdup(input_str);
	if (!str_dup)
		return (NULL);
	
	str_dup = add_spaces_around_redirections(str_dup);
	if (!str_dup)
		return NULL;
	args = smart_split_and_parse(command, str_dup);
	free(str_dup);
	return (args);
}