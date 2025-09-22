/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_utils_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:13 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/18 07:35:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

void	init_parse_context(t_parse_context *ctx, char *src)
{
	ctx->i = 0;
	ctx->j = 0;
	ctx->src_len = ft_strlen(src);
}

int	should_handle_env(char *src, int i)
{
	return (src[i] == '$' && ft_strcmp(src, "cd"));
}

void	process_character(t_cmd *cmd, char *src, char *dest,
		t_parse_context *ctx)
{
	dest[ctx->j++] = src[ctx->i++];
	(void)cmd;
}

char	*extract_raw_arg(const char *str, int start, int len)
{
	char	*raw_arg;

	raw_arg = malloc(len + 1);
	if (!raw_arg)
		return (NULL);
	ft_strlcpy(raw_arg, str + start, len + 1);
	return (raw_arg);
}

char	*create_final_arg(char *parsed_arg)
{
	char	*final_arg;

	final_arg = ft_strdup(parsed_arg);
	free(parsed_arg);
	return (final_arg);
}
