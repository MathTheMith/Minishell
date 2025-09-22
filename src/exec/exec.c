/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:21 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/18 08:52:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "check_invalid.h"
#include "exec_process.h"

int check_info(t_cmd *cmds, t_cmd **command, char **envp)
{
    t_cmd *cur;

    cur = cmds;
    while (cur)
    {
        if (cur->args && cur->args[0])
        {
            if (check_invalid_start(cur->args[0], 0) == EXIT_FAILURE)
            {
                free_all_cmds(cmds, 1);
                return (-1);
            }
        }
        cur = cur->next;
    }

    if (cmds && cmds->next)
        exec_pipeline(cmds, envp);
    else
        {check_args(cmds, command, envp);}

	free_envp(envp);
    return (1);
}

