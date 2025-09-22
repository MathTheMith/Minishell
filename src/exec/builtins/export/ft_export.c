/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomm <tomm@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/15 17:13:50 by tomm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

int	print_error(char *msg)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	return (1);
}

static void	export_input_helper(t_cmd *cmds, int i)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(cmds->args[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	cmds->last_exit_code = 1;
}

int	export_input(t_cmd *cmds, t_list **env)
{
	int	i;

	i = 1;
	if (!cmds->args || !cmds->args[i])
	{
		if (*env && !export_no_args(*env))
			return (print_error(ERR_MALLOC));
		return (0);
	}
	cmds->args[i] = put_last_exit_code(cmds->args[i], cmds);
	while (cmds->args[i])
	{
		if (cmds->args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		if (!valid_identifier(cmds->args[i]))
			export_input_helper(cmds, i);
		else if (!export_var(cmds->args[i], env))
			return (print_error(ERR_MALLOC));
		i++;
	}
	return (0);
}
