/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:29 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd_input(t_cmd *cmds)
{
    char cwd[1024];

    if (cmds->args && cmds->args[0] && ft_strcmp(cmds->args[0], "pwd") == 0)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            ft_putstr_fd(cwd, 1);
            ft_putstr_fd("\n", 1);
        }
        else
            perror("pwd");
    }
}