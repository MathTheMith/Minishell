/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_extract.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:10 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:05:11 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

void	init_arg_info(t_arg_info *info, const char *str, int pos)
{
	info->start = pos;
	info->len = get_arg_length(str, info->start, &info->quote_type);
}

char	*extract_and_parse_arg(t_cmd *cmd, char *str, int *pos, int arg_quoted)
{
	t_arg_info	info;
	char		*raw_arg;
	char		*parsed_arg;

	skip_spaces(str, pos);
	init_arg_info(&info, str, *pos);
	if (cmd && cmd->quoted)
		cmd->quoted[arg_quoted] = info.quote_type;
	*pos += info.len;
	raw_arg = extract_raw_arg(str, info.start, info.len);
	if (!raw_arg)
		return (NULL);
	parsed_arg = malloc(info.len * 2 + 512);
	if (!parsed_arg)
	{
		free(raw_arg);
		return (NULL);
	}
	parse_input_string(cmd, raw_arg, parsed_arg);
	free(raw_arg);
	return (create_final_arg(parsed_arg));
}
