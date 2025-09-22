/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:58 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/15 18:53:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

void print_data(t_cmd **command)
{
 	// (void)command;
	int i = 0;
	int j;
	printf("Data: \n");
	printf(" |nb_cmd: %d\n", command[0]->data->nb_cmd);
	printf(" |input = %s\n\n", command[0]->data->input);
	printf("\n");
	while (i < command[0]->data->nb_cmd)
	{
		printf("CMD[%d]: \n", i);
		printf("   |id: %d\n", command[i]->id);
		printf("   |args: ");
		j = 0;
		if (command[i]->args)
		{
			while (command[i]->args[j])
			{
				printf("%s ", command[i]->args[j]);
				j++;
			}
		}
		printf("\n");
		printf("   |quoted: ");
		j = 0;
		while (command[i]->nb_args > j)
		{
			printf("%d ", command[i]->quoted[j]);
			j++;
		}
		printf("\n");
		printf("   |nb_args: %d\n", command[i]->nb_args);
		printf("   |infile: %s\n", command[i]->infile);
		printf("   |ascending: %d\n", command[i]->ascending);
		printf("   |outfile: %s\n", command[i]->outfile);
		printf("   |descending: %d\n", command[i]->descending);
		printf("   |pipe_in: %d\n", command[i]->pipe_in);
		printf("   |pipe_out: %d\n\n", command[i]->pipe_out);
		i++;
	}
}
