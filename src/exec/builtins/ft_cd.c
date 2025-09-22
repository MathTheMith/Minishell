/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:05 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:06 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cd.h"
#include "ft_echo.h"

static int	validate_cd_args(t_cmd *cmds)
{
	int	arg_count;

	arg_count = 0;
	if (cmds->args)
	{
		while (cmds->args[arg_count + 1])
			arg_count++;
	}
	if (arg_count > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		cmds->last_exit_code = 1;
		return (1);
	}
	return (0);
}

static int	save_current_dir(t_cd_data *data, t_cmd *cmds)
{
	if (!getcwd(data->oldpwd, sizeof(data->oldpwd)))
	{
		perror("getcwd");
		cmds->last_exit_code = 1;
		return (1);
	}
	return (0);
}

static char	*get_target_dir(t_cmd *cmds)
{
	char	*dir;

	if (!cmds->args[1])
	{
		dir = get_env_value(cmds->env, "HOME");
		if (!dir)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			cmds->last_exit_code = 1;
			return (NULL);
		}
	}
	else
		dir = cmds->args[1];
	dir = expand_tilde(dir, cmds);
	if (!dir)
		return (NULL);
	dir = resolve_env_var(dir, cmds);
	return (dir);
}

static int	change_directory(char *dir, t_cmd *cmds)
{
	if (chdir(dir) != 0)
	{
		perror("cd");
		cmds->last_exit_code = 1;
		return (1);
	}
	return (0);
}

void	cd_input(t_cmd *cmds)
{
	t_cd_data	data;
	char		*target_dir;

	data.resolved_dir = NULL;
	if (validate_cd_args(cmds) != 0)
		return ;
	if (save_current_dir(&data, cmds) != 0)
		return ;
	target_dir = get_target_dir(cmds);
	if (!target_dir)
		return ;
	if (change_directory(target_dir, cmds) != 0)
		return ;
	if (getcwd(data.cwd, sizeof(data.cwd)))
	{
		add_to_env(&cmds->env, "OLDPWD", data.oldpwd);
		add_to_env(&cmds->env, "PWD", data.cwd);
	}
	cmds->last_exit_code = 0;
}
