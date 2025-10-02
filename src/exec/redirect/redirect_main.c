/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:02:38 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 19:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

void	write_syntax_error(char *token)
{
	write(2, "minishell: syntax error near unexpected token `", 48);
	write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
}

static int	find_last_input_redirect(char **args)
{
	int	i;
	int	last;

	i = 0;
	last = -1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
			last = i;
		i++;
	}
	return (last);
}

static int	apply_heredoc(char *content, t_cmd *cmd)
{
	int		pipefd[2];
	size_t	len;

	if (!content)
		return (0);
	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		cmd->last_exit_code = 1;
		return (-1);
	}
	len = ft_strlen(content);
	if (len > 0)
		write(pipefd[1], content, len);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}

typedef struct s_redirect_ctx
{
	int		heredoc_idx;
	int		last_input;
}	t_redirect_ctx;

static int	handle_redirect_type(t_cmd *cmd, char **args, int i,
				t_redirect_ctx *ctx)
{
	int	result;

	if (ft_strcmp(args[i], ">") == 0)
		return (handle_output_redirect(cmd, args[i + 1]));
	else if (ft_strcmp(args[i], ">>") == 0)
		return (handle_append_redirect(cmd, args[i + 1]));
	else if (ft_strcmp(args[i], "<") == 0)
	{
		if (i == ctx->last_input)
			return (handle_input_redirect(cmd, args[i + 1]));
		return (0);
	}
	else if (ft_strcmp(args[i], "<<") == 0)
	{
		if (i == ctx->last_input && cmd->heredoc_buffer
			&& ctx->heredoc_idx < cmd->heredoc_count)
		{
			result = apply_heredoc(cmd->heredoc_buffer[ctx->heredoc_idx], cmd);
			ctx->heredoc_idx++;
			return (result);
		}
		ctx->heredoc_idx++;
		return (0);
	}
	return (0);
}

static int	process_redirect_arg(t_cmd *cmd, char **args, int *i,
				t_redirect_ctx *ctx)
{
	int	result;

	if (ft_strcmp(args[*i], ">") == 0 || ft_strcmp(args[*i], ">>") == 0
		|| ft_strcmp(args[*i], "<") == 0 || ft_strcmp(args[*i], "<<") == 0)
	{
		if (!args[*i + 1])
		{
			write_syntax_error("newline");
			return (-1);
		}
		result = handle_redirect_type(cmd, args, *i, ctx);
		if (result < 0)
			return (-1);
		*i += 2;
	}
	else
		(*i)++;
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	int				i;
	t_redirect_ctx	ctx;

	if (!cmd || !cmd->args)
		return (0);
	if (bad_redirection(cmd->args) < 0)
	{
		cmd->last_exit_code = 2;
		return (-1);
	}
	ctx.heredoc_idx = 0;
	ctx.last_input = find_last_input_redirect(cmd->args);
	i = 0;
	while (cmd->args[i])
	{
		if (process_redirect_arg(cmd, cmd->args, &i, &ctx) < 0)
			return (-1);
	}
	return (0);
}