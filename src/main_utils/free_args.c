/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 22:28:27 by marvin            #+#    #+#             */
/*   Updated: 2025/09/11 22:28:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "path_exec.h"

void	free_cleaned_args(char **cleaned_args)
{
	int	i;

	if (!cleaned_args)
		return ;
	i = 0;
	while (cleaned_args[i])
	{
		free(cleaned_args[i]);
		i++;
	}
	free(cleaned_args);
	cleaned_args = NULL;
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	free_args_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

void	free_clean_args(char **new_args, int j)
{
	while (--j >= 0)
		free(new_args[j]);
	free(new_args);
	new_args = NULL;
}
