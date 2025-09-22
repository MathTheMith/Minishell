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
}

void	free_clean_args(char **new_args, int j)
{
	while (--j >= 0)
		free(new_args[j]);
	free(new_args);
}
