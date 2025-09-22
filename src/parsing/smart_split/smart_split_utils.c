/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:16 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:05:17 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

void	parse_input_string(t_cmd *cmd, char *src, char *dest)
{
	t_parse_context	ctx;
	t_quote_params	params;

	init_parse_context(&ctx, src);
	params.str = src;
	params.out = dest;
	params.j = &ctx.i;
	params.k = &ctx.j;
	while (ctx.i < ctx.src_len)
	{
		if (src[ctx.i] == '\'')
			handle_single_quotes(&params);
		else if (src[ctx.i] == '\"')
			handle_double_quotes(cmd, &params);
		else if (should_handle_env(src, ctx.i))
			handle_env_variable(cmd, &params);
		else
			process_character(cmd, src, dest, &ctx);
	}
	dest[ctx.j] = '\0';
}

void	skip_spaces(const char *str, int *i)
{
	while (str[*i] && str[*i] == ' ')
		(*i)++;
}

void	update_quote_state(const char c, int *in_quotes, char *quote)
{
	if (!*in_quotes && (c == '\'' || c == '\"'))
	{
		*in_quotes = 1;
		*quote = c;
	}
	else if (*in_quotes && c == *quote)
	{
		*in_quotes = 0;
		*quote = 0;
	}
}

int	count_string_args(const char *str)
{
	int				count;
	int				i;
	t_quote_state	qs;

	count = 0;
	i = 0;
	qs.in_quotes = 0;
	qs.quote = 0;
	while (str[i])
	{
		skip_spaces(str, &i);
		if (!str[i])
			break ;
		count++;
		while (str[i] && (str[i] != ' ' || qs.in_quotes))
		{
			update_quote_state(str[i], &qs.in_quotes, &qs.quote);
			i++;
		}
	}
	return (count);
}
