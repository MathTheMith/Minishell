/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:42 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:43 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

static char	*extract_command(char *input, int start, int end)
{
	char	*cmd;
	int		len;

	len = end - start;
	cmd = malloc(len + 1);
	if (!cmd)
		return (NULL);
	ft_strlcpy(cmd, input + start, len + 1);
	return (cmd);
}

static void	init_split_context(t_split_context *ctx)
{
	ctx->cmd_index = 0;
	ctx->start = 0;
	ctx->i = 0;
	ctx->in_single_quote = 0;
	ctx->in_double_quote = 0;
}

static int	handle_pipe_found(char *input, char **result, t_split_context *ctx)
{
	result[ctx->cmd_index] = extract_command(input, ctx->start, ctx->i);
	if (!result[ctx->cmd_index])
	{
		free_result_array(result, ctx->cmd_index);
		return (0);
	}
	ctx->cmd_index++;
	ctx->start = ctx->i + 1;
	return (1);
}

static int	process_pipe_split(char *input, char **result, int pipe_count)
{
	t_split_context	ctx;

	(void)pipe_count;
	init_split_context(&ctx);
	while (input[ctx.i])
	{
		update_quote_state_pipe(input[ctx.i], &ctx.in_single_quote,
			&ctx.in_double_quote);
		if (input[ctx.i] == '|' && !ctx.in_single_quote && !ctx.in_double_quote)
		{
			if (!handle_pipe_found(input, result, &ctx))
				return (0);
		}
		ctx.i++;
	}
	result[ctx.cmd_index] = extract_command(input, ctx.start, ctx.i);
	if (!result[ctx.cmd_index])
	{
		free_result_array(result, ctx.cmd_index);
		return (0);
	}
	return (1);
}

char	**split_by_pipes(char *input)
{
	char	**result;
	int		pipe_count;

	pipe_count = count_pipes(input);
	result = malloc((pipe_count + 2) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!process_pipe_split(input, result, pipe_count))
		return (NULL);
	result[pipe_count + 1] = NULL;
	return (result);
}
