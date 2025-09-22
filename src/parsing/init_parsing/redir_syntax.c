/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:44 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:04:45 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

static int	check_invalid_combination(t_redir_ctx *ctx)
{
	const char	*input = ctx->input;
	int			i;

	i = ctx->i;
	if ((input[i] == '>' && input[i + 1] == '<') || (input[i] == '<' && input[i
				+ 1] == '>'))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(input[i], 2);
		ft_putchar_fd(input[i + 1], 2);
		ft_putstr_fd("'\n", 2);
		free(ctx->result);
		return (0);
	}
	return (1);
}

static int	check_too_many_redirections(t_redir_ctx *ctx, char type, int count)
{
	if (count > 2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (type == '>')
			ft_putstr_fd(">>", 2);
		else
			ft_putstr_fd("<<", 2);
		ft_putstr_fd("'\n", 2);
		free(ctx->result);
		return (0);
	}
	return (1);
}

static int	check_consecutive_different_redir(t_redir_ctx *ctx, char type,
		int count)
{
	const char	*input = ctx->input;
	int			i;
	char		next_after_redirections;

	i = ctx->i;
	next_after_redirections = input[i + count];
	if (next_after_redirections && (next_after_redirections == '>'
			|| next_after_redirections == '<')
		&& next_after_redirections != type)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(next_after_redirections, 2);
		ft_putstr_fd("'\n", 2);
		free(ctx->result);
		return (0);
	}
	return (1);
}

int	check_redir_syntax(t_redir_ctx *ctx, char type)
{
	const char	*input = ctx->input;
	int			i;
	int			count;

	i = ctx->i;
	if (!check_invalid_combination(ctx))
		return (0);
	count = 1;
	while (input[i + count] == type)
		count++;
	if (!check_too_many_redirections(ctx, type, count))
		return (0);
	if (!check_consecutive_different_redir(ctx, type, count))
		return (0);
	return (count);
}
