/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:53 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:54 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections_parsing.h"

char	*check_outfile(char *processed)
{
	char	**cmd_line;
	char	*result;
	int		j;

	j = 0;
	result = NULL;
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (NULL);
	while (cmd_line[j])
	{
		if ((ft_strcmp(cmd_line[j], ">") == 0 || ft_strcmp(cmd_line[j],
					">>") == 0) && cmd_line[j + 1])
		{
			result = ft_strdup(cmd_line[j + 1]);
			break ;
		}
		j++;
	}
	j = -1;
	while (cmd_line[++j])
		free(cmd_line[j]);
	free(cmd_line);
	return (result);
}

int	check_descending(char *processed)
{
	char	**cmd_line;
	int		result;
	int		j;

	result = 0;
	j = 0;
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (0);
	while (cmd_line[j])
	{
		if (ft_strcmp(cmd_line[j], ">>") == 0)
			result = 2;
		else if (ft_strcmp(cmd_line[j], ">") == 0)
			result = 1;
		j++;
	}
	j = 0;
	while (cmd_line[j])
	{
		free(cmd_line[j]);
		j++;
	}
	free(cmd_line);
	return (result);
}
