/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:24:22 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:24:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipeline.h"
#include "exec_cmd_utils.h"
#include "path_exec.h"
#include "redirections.h"

void	handle_builtin_in_pipeline(t_cmd *current, char **cleaned_args,
			t_cmd *cmds, char **envp)
{
	int			exit_code;
	char		**original_args;
	long long	num;

	if (ft_strcmp(cleaned_args[0], "exit") == 0)
	{
		if (!cleaned_args[1])
			exit_code = 0;
		else if (!ft_atoll_safe(cleaned_args[1], &num))
			exit_code = 2;
		else if (cleaned_args[2])
			exit_code = 1;
		else
			exit_code = (unsigned char)(num % 256);
		cleanup_and_exit_child(cmds, envp, cleaned_args, exit_code);
	}
	original_args = current->args;
	current->args = cleaned_args;
	current->pipe_in = (void *)1;
	exec_builtin(current, envp);
	exit_code = cmds->last_exit_code;
	current->args = original_args;
	cleanup_and_exit_child(cmds, envp, cleaned_args, exit_code);
}

void	handle_external_in_pipeline(t_cmd *cmds, char **envp,
			char **cleaned_args)
{
	char	*path;

	path = find_path(cmds, cleaned_args[0], envp);
	if (!path)
	{
		ft_putstr_fd(cleaned_args[0], 2);
		ft_putstr_fd(": command not found \n", 2);
		cleanup_and_exit_child(cmds, envp, cleaned_args, 127);
	}
	execve(path, cleaned_args, envp);
	perror("execve");
	free(path);
	cleanup_and_exit_child(cmds, envp, cleaned_args, 126);
}

void	exec_pipeline_child(t_cmd *current, t_cmd *cmds,
			int prev_fd, int *pipefd)
{
	char	**cleaned_args;

	setup_pipes(prev_fd, pipefd, current);
	if (handle_redirections(current) < 0)
		cleanup_and_exit_child(cmds, cmds->envp, NULL, 2);
	cleaned_args = remove_redirections(current->args);
	if (!cleaned_args)
		cleanup_and_exit_child(cmds, cmds->envp, NULL, 1);
	if (is_builtin(current))
		handle_builtin_in_pipeline(current, cleaned_args, cmds, cmds->envp);
	else
		handle_external_in_pipeline(cmds, cmds->envp, cleaned_args);
}

int	process_single_cmd(t_cmd *current, t_cmd *cmds, char **envp,
			int *prev_fd)
{
	int		pipefd[2];
	pid_t	pid;

	cmds->envp = envp;
	if (create_pipe_if_needed(current, pipefd) == -1)
		return (-1);
	pid = fork_and_handle_child(current, cmds, *prev_fd, pipefd);
	if (pid == -1)
		return (-1);
	handle_parent_cleanup(*prev_fd, pipefd, current);
	if (current->next)
		*prev_fd = pipefd[0];
	return (pid);
}
