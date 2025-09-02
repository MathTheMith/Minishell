/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:14:48 by mvachon           #+#    #+#             */
/*   Updated: 2025/07/25 19:00:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_vars(t_cmd *cmds)
{
    int i = 0;
    while (cmds->args[i])
    {
        if (cmds->args[i][0] == '$')
        {
            char *env_val = getenv(cmds->args[i] + 1);
            if (env_val)
            {
                free(cmds->args[i]);
                cmds->args[i] = strdup(env_val);
            }
            else
            {
                free(cmds->args[i]);
                cmds->args[i] = strdup("");
            }
        }
        i++;
    }
}


