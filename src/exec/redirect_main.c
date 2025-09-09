/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

void	write_syntax_error(char *token)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
}

static int	handle_redirect_type(t_cmd *cmd, char *redirect, char *filename,
			int *heredoc_found)
{
	if (ft_strcmp(redirect, ">") == 0)
	{
		if (handle_output_redirect(cmd, filename) < 0)
			return (-1);
	}
	else if (ft_strcmp(redirect, ">>") == 0)
	{
		if (handle_append_redirect(cmd, filename) < 0)
			return (-1);
	}
	else if (ft_strcmp(redirect, "<") == 0)
	{
		if (handle_input_redirect(cmd, filename) < 0)
			return (-1);
	}
	else if (ft_strcmp(redirect, "<<") == 0)
	{
		if (handle_heredoc(cmd, filename) < 0)
			return (-1);
		*heredoc_found = 1;
	}
	else
		return (0);
	return (1);
}

static int	process_redirect_arg(t_cmd *cmd, char **args, int *i,
				int *heredoc_found)
{
	int	result;

	result = handle_redirect_type(cmd, args[*i], args[*i + 1], heredoc_found);
	if (result == -1)
		return (-1);
	if (result == 1)
		*i += 2;
	else
		(*i)++;
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	int	i;
	int	heredoc_found;

	if (!cmd || !cmd->args)
		return (0);
	if (bad_redirection(cmd->args) < 0)
	{
		cmd->last_exit_code = 2;
		return (-1);
	}
	i = 0;
	heredoc_found = 0;
	while (cmd->args[i])
	{
		if (process_redirect_arg(cmd, cmd->args, &i, &heredoc_found) < 0)
			return (-1);
	}
	return (0);
}
