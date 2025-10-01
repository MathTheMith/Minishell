/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:22:32 by marvin            #+#    #+#             */
/*   Updated: 2025/09/30 02:42:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

void	free_partial_commands(t_cmd **command, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (command[i])
		{
			free_one_cmd(command[i], 1);
		}
		i++;
	}
	free(command);
}

static void	free_cmd_array_helper(t_cmd **command, int index)
{
	if (command[index]->quoted)
		free(command[index]->quoted);
	if (command[index]->infile)
		free(command[index]->infile);
	if (command[index]->outfile)
		free(command[index]->outfile);
	free(command[index]);
}

static void	free_cmd_array(t_cmd **command, int failed_idx)
{
	int	j;
	int	i;

	j = 0;
	while (j < failed_idx)
	{
		if (command[j])
		{
			if (command[j]->args)
			{
				i = 0;
				while (command[j]->args[i])
				{
					if (command[j]->args[i])
						free(command[j]->args[i]);
					i++;
				}
				free(command[j]->args);
			}
			free_cmd_array_helper(command, j);
		}
		j++;
	}
	free(command);
}

void	free_failed_init(t_cmd **command, int failed_idx, char **cmd_pipe)
{
	int	j;

	if (command)
		free_cmd_array(command, failed_idx);
	j = -1;
	if (!cmd_pipe)
		return ;
	while (cmd_pipe[++j])
		free(cmd_pipe[j]);
	free(cmd_pipe);
}
