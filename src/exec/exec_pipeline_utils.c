/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipeline.h"
#include "exec_cmd_utils.h"

int	count_commands(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		cmd_count;

	tmp = cmds;
	cmd_count = 0;
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	return (cmd_count);
}

void	setup_pipes(int prev_fd, int *pipefd, t_cmd *current)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	cleanup_pipes(int prev_fd, int *pipefd, t_cmd *current)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next)
		close(pipefd[1]);
}

void	wait_pipeline_processes(pid_t *pids, int cmd_count, t_cmd *cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				cmds->last_exit_code = WEXITSTATUS(status);
			else
				cmds->last_exit_code = 1;
		}
		i++;
	}
}

void	handle_parent_cleanup(int prev_fd, int *pipefd, t_cmd *current)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next)
		close(pipefd[1]);
}
