/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spc_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:47 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:48 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

static int	handle_quotes(t_redir_ctx *ctx, int *in_quotes, char *quote_char)
{
	if ((ctx->input[ctx->i] == '\'' || ctx->input[ctx->i] == '"')
		&& !(*in_quotes))
	{
		*in_quotes = 1;
		*quote_char = ctx->input[ctx->i];
		ctx->result[(ctx->j)++] = ctx->input[(ctx->i)++];
		return (1);
	}
	else if (*in_quotes && ctx->input[ctx->i] == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = 0;
		ctx->result[(ctx->j)++] = ctx->input[(ctx->i)++];
		return (1);
	}
	return (0);
}

static void	add_spaces_for_redir(t_redir_ctx *ctx, int count, char type)
{
	int	k;

	k = 0;
	if (ctx->j > 0 && ctx->result[ctx->j - 1] != ' ')
		ctx->result[(ctx->j)++] = ' ';
	while (k < count)
	{
		ctx->result[(ctx->j)++] = type;
		k++;
	}
	ctx->i += count;
	if (ctx->input[ctx->i] && ctx->input[ctx->i] != ' ')
		ctx->result[(ctx->j)++] = ' ';
}

static int	init_redir_ctx(t_redir_ctx *ctx, char *input)
{
	ctx->input = input;
	ctx->i = 0;
	ctx->j = 0;
	ctx->count = 0;
	ctx->type = 0;
	ctx->result = malloc(ft_strlen(input) * 2 + 1);
	if (!ctx->result)
		return (0);
	return (1);
}

char	*add_spaces_around_redirections(char *input)
{
	t_redir_ctx	ctx;
	int			in_quotes;
	char		quote_char;

	if (!init_redir_ctx(&ctx, input))
		return (NULL);
	in_quotes = 0;
	quote_char = 0;
	while (ctx.input[ctx.i])
	{
		if (handle_quotes(&ctx, &in_quotes, &quote_char))
			continue ;
		if (!in_quotes && (ctx.input[ctx.i] == '>' || ctx.input[ctx.i] == '<'))
		{
			ctx.type = ctx.input[ctx.i];
			ctx.count = check_redir_syntax(&ctx, ctx.type);
			if (ctx.count == 0)
				return (NULL);
			add_spaces_for_redir(&ctx, ctx.count, ctx.type);
		}
		else
			ctx.result[(ctx.j)++] = ctx.input[(ctx.i)++];
	}
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
