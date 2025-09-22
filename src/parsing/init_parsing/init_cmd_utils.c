/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomm <tomm@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:04:02 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/15 16:50:45 by tomm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_parsing.h"

int	count_cmd(t_data *data)
{
	int	i;
	int	nb_cmd;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	nb_cmd = 1;
	in_single_quote = 0;
	in_double_quote = 0;
	while (data->input[i])
	{
		if (data->input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (data->input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (data->input[i] == '|' && !in_single_quote && !in_double_quote)
			nb_cmd++;
		i++;
	}
	return (nb_cmd);
}

int	count_args(char *processed)
{
	char	**split;
	int		count;
	int		i;

	split = ft_split(processed, ' ');
	if (!split)
		return (0);
	count = 0;
	while (split[count])
		count++;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (count);
}
