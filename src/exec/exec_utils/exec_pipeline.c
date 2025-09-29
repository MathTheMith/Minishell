/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:42 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:42 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipeline.h"

static pid_t	*allocate_pid_array(int cmd_count)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("malloc");
		return (NULL);
	}
	return (pids);
}

int	create_pipe_if_needed(t_cmd *current, int *pipefd)
{
	if (current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

pid_t	fork_and_handle_child(t_cmd *current, t_cmd *cmds,
				int prev_fd, int *pipefd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, sigquit_handler);
		exec_pipeline_child(current, cmds, prev_fd, pipefd);
	}
	return (pid);
}

static int	execute_commands_loop(t_cmd *cmds, char **envp, pid_t *pids)
{
	t_cmd	*current;
	int		prev_fd;
	int		cmd_index;
	pid_t	pid;

	current = cmds;
	prev_fd = -1;
	cmd_index = 0;
	while (current)
	{
		pid = process_single_cmd(current, cmds, envp, &prev_fd);
		if (pid == -1)
		{
			free(pids);
			return (-1);
		}
		pids[cmd_index++] = pid;
		current = current->next;
	}
	return (1);
}

void	exec_pipeline(t_cmd *cmds, char **envp)
{
	pid_t	*pids;
	int		cmd_count;

	cmd_count = count_commands(cmds);
	pids = allocate_pid_array(cmd_count);
	if (!pids)
		return ;
	if (execute_commands_loop(cmds, envp, pids) == -1)
		return ;
	wait_pipeline_processes(pids, cmd_count, cmds);
	free(pids);
}
