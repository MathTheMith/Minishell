/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:19 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:20 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_input(t_cmd *cmds)
{
	char	cwd[1024];

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
