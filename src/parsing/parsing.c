/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:18 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/15 19:51:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_invalid.h"
#include "init_parsing.h"
#include "minishell.h"

int	parsing(char *input, t_cmd ***command, t_list *env_list, int lec)
{
	t_data	*data;

	if (check_invalid(input) == EXIT_FAILURE)
		return (-1);
	data = malloc(sizeof(t_data));
	ft_memset(data, 0, sizeof(t_data));
	data->input = input;
	data->lec_save = lec;
	init_command(command, data, env_list);
	if (!*command)
	{
		free(data);
		return (-1);
	}
	return (0);
}
