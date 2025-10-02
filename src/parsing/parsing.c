/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:18 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 04:15:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"
#include "init_parsing.h"
#include "minishell.h"
#include "redirections.h"

static int	init_heredoc_storage(t_cmd *cmd, char *buffer)
{
	if (!cmd->heredoc_buffer)
	{
		cmd->heredoc_buffer = malloc(sizeof(char *) * 16);
		if (!cmd->heredoc_buffer)
		{
			free(buffer);
			return (-1);
		}
		cmd->heredoc_count = 0;
	}
	cmd->heredoc_buffer[cmd->heredoc_count] = buffer;
	cmd->heredoc_count++;
	cmd->heredoc_buffer[cmd->heredoc_count] = NULL;
	return (0);
}

static int	read_and_store_heredoc(t_cmd *cmd, char *delimiter)
{
	char	*buffer;
	size_t	bufsize;
	char	*clean_delim;
	int		ret;

	buffer = NULL;
	bufsize = 0;
	clean_delim = remove_quotes_from_delimiter(delimiter);
	if (!clean_delim)
		return (-1);
	ret = read_heredoc_loop(clean_delim, cmd, &buffer, &bufsize);
	free(clean_delim);
	if (!ret)
	{
		free(buffer);
		cmd->last_exit_code = 130;
		return (-1);
	}
	return (init_heredoc_storage(cmd, buffer));
}

static int	read_all_heredocs_for_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
		return (0);
	i = 0;
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], "<<") == 0 && cmd->args[i + 1])
		{
			if (read_and_store_heredoc(cmd, cmd->args[i + 1]) < 0)
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	read_all_heredocs_for_pipeline(t_cmd **command, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_cmd)
	{
		if (read_all_heredocs_for_cmd(command[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	parsing(char *input, t_cmd ***command, t_list *env_list, int lec)
{
	t_data	*data;

	if (check_invalid(input) == EXIT_FAILURE)
		return (-1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (-1);
	ft_memset(data, 0, sizeof(t_data));
	data->input = input;
	data->lec_save = lec;
	init_command(command, data, env_list);
	if (!*command)
	{
		free(data);
		return (-1);
	}
	if (read_all_heredocs_for_pipeline(*command, data) < 0)
	{
		free_partial_commands(*command, data->nb_cmd);
		free(data);
		*command = NULL;
		g_interrupt = 0;
		return (-1);
	}
	return (0);
}