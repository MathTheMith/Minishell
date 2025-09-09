/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd_utils.h"

char	*get_all_cmd(t_cmd *cmds)
{
	char	*joined;
	char	*tmp;
	char	*new_join;
	int		i;

	if (!cmds->args || !cmds->args[0])
		return (NULL);
	joined = ft_strdup(cmds->args[0]);
	if (!joined)
		return (NULL);
	i = 1;
	while (cmds->args[i])
	{
		tmp = ft_strjoin(joined, " ");
		free(joined);
		if (!tmp)
			return (NULL);
		new_join = ft_strjoin(tmp, cmds->args[i]);
		free(tmp);
		if (!new_join)
			return (NULL);
		joined = new_join;
		i++;
	}
	return (joined);
}

int	is_builtin(t_cmd *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	if (ft_strcmp(cmds->args[0], "exit") == 0
		|| ft_strcmp(cmds->args[0], "cd") == 0
		|| ft_strcmp(cmds->args[0], "echo") == 0
		|| ft_strcmp(cmds->args[0], "export") == 0
		|| ft_strcmp(cmds->args[0], "unset") == 0
		|| ft_strcmp(cmds->args[0], "env") == 0
		|| ft_strcmp(cmds->args[0], "pwd") == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_cmd *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return ;
	if (ft_strcmp(cmds->args[0], "exit") == 0)
	{
		if (cmds->pipe_in)
			exit_input_pipeline(cmds);
		else
			exit_input(cmds);
	}
	else if (ft_strcmp(cmds->args[0], "cd") == 0)
		cd_input(cmds);
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		echo_input(cmds);
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		export_input(cmds, &(cmds->env));
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		unset_input(cmds, &cmds->env);
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		env_input(cmds);
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		pwd_input(cmds);
}

int	is_path_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '/')
		return (1);
	if (cmd[0] == '.' && cmd[1] == '/')
		return (1);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}

void	handle_path_error(t_cmd *cmds, char *cmd_name)
{
	struct stat	st;

	if (stat(cmd_name, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			cmds->last_exit_code = 126;
		else if (!(st.st_mode & S_IXUSR))
			cmds->last_exit_code = 126;
		else
		{
			cmds->last_exit_code = 126;
			ft_putstr_fd(cmd_name, 2);
			ft_putstr_fd(": cannot execute binary file\n", 2);
		}
	}
	else
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmds->last_exit_code = 127;
	}
}
