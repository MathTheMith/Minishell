/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:22:46 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/02 12:15:16 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parsing(char *input, t_cmd ***command, t_list *env_list, int lec)
{
	t_data *data;

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