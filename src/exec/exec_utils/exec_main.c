/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:04:02 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 15:04:02 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_process.h"
#include "exec_cmd_utils.h"
#include "redirections.h"

static int	handle_special_cases(t_cmd *current, t_cmd *cmds)
{
	if (ft_strcmp(current->args[0], "..") == 0)
	{
		if (handle_redirections(current) < 0)
			cmds->last_exit_code = 2;
		else
		{
			ft_putstr_fd(current->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			cmds->last_exit_code = 127;
		}
		return (1);
	}
	return (0);
}

static int	handle_command_not_found(t_cmd *current)
{
	if (is_path_command(current->args[0]))
		handle_path_error(current, current->args[0]);
	else
	{
		if (handle_redirections(current) < 0)
		{
			ft_putstr_fd(current->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			current->last_exit_code = 127;
		}
	}
	return (1);
}

static void	backup_and_restore_fds(int *stdin_backup, int *stdout_backup,
				int restore)
{
	if (restore)
	{
		dup2(*stdin_backup, STDIN_FILENO);
		dup2(*stdout_backup, STDOUT_FILENO);
		close(*stdin_backup);
		close(*stdout_backup);
	}
	else
	{
		*stdin_backup = dup(STDIN_FILENO);
		*stdout_backup = dup(STDOUT_FILENO);
	}
}

static int	process_single_command(t_cmd *current, t_cmd *cmds,
				t_cmd **command, char **envp)
{
	t_cmd_exec	*cmd;
	int			ret;

	if (handle_special_cases(current, cmds))
		return (1);
	ret = handle_redirections(current);
	if (ret != 0)
		return (1);
	if (is_builtin(current))
	{
		exec_builtin_free(current, command, envp);
		return (1);
	}
	cmd = init_cmd_exec(cmds, current->args[0], envp);
	if (!cmd)
		return (handle_command_not_found(current));
	execute_cmd(cmd, envp, current->args, current);
	free_cmd(cmd);
	return (1);
}

int	check_args(t_cmd *cmds, t_cmd **command, char **envp)
{
	t_cmd	*current;
	int		stdin_backup;
	int		stdout_backup;

	current = cmds;
	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue ;
		}
		backup_and_restore_fds(&stdin_backup, &stdout_backup, 0);
		process_single_command(current, cmds, command, envp);
		backup_and_restore_fds(&stdin_backup, &stdout_backup, 1);
		current = current->next;
	}
	return (1);
}
