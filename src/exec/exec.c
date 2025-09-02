/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:23:39 by mvachon           #+#    #+#             */
/*   Updated: 2025/08/29 12:24:25 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_info(t_cmd *cmds, char **envp)
{
	t_cmd *cur;

	cur = cmds;
	while (cur) 
	{
		if (cur->args && cur->args[0])
		{
			if (check_start(cur->args[0], 0) == EXIT_FAILURE)
				return (-1);
		}
		cur = cur->next;
	}

	if (cmds && cmds->next)
		exec_pipeline(cmds, envp);
	else
		check_args(cmds, envp);
	return (1);
}
