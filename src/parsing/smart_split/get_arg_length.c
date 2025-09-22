/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_length.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:03 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:05:04 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

static void	handle_quote_detection(const char *str, int i, int *quote_type,
		t_quote_state *qs)
{
	if (!qs->in_quotes && (str[i] == '\'' || str[i] == '\"'))
	{
		qs->in_quotes = 1;
		qs->quote = str[i];
		if (quote_type)
		{
			if (str[i] == '\'')
				*quote_type = 1;
			else
				*quote_type = 2;
		}
	}
	else if (qs->in_quotes && str[i] == qs->quote)
	{
		qs->in_quotes = 0;
		qs->quote = 0;
	}
}

int	get_arg_length(const char *str, int start, int *quote_type)
{
	int				i;
	int				len;
	t_quote_state	qs;

	i = start;
	len = 0;
	qs.in_quotes = 0;
	qs.quote = 0;
	while (str[i] && (str[i] != ' ' || qs.in_quotes))
	{
		handle_quote_detection(str, i, quote_type, &qs);
		i++;
		len++;
	}
	return (len);
}
