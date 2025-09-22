/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:38 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:38 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_pipeline.h"
#include "exec_cmd_utils.h"
#include "path_exec.h"
#include "redirections.h"

static void	free_cleaned_args_and_exit(char **cleaned_args, int exit_code)
{
	int	i;

	if (cleaned_args)
	{
		i = 0;
		while (cleaned_args[i])
		{
			free(cleaned_args[i]);
			i++;
		}
		free(cleaned_args);
	}
	exit(exit_code);
}

void	handle_builtin_in_pipeline(t_cmd *current, char **cleaned_args,
			t_cmd *cmds, char **envp)
{
	t_cmd	temp_cmd;

	temp_cmd = *current;
	temp_cmd.args = cleaned_args;
	temp_cmd.pipe_in = (void *)1;
	exec_builtin(&temp_cmd, envp);
	free_cleaned_args_and_exit(cleaned_args, cmds->last_exit_code);
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
		free_cleaned_args_and_exit(cleaned_args, 127);
	}
	execve(path, cleaned_args, envp);
	perror("execve");
	free(path);
	free_cleaned_args_and_exit(cleaned_args, 126);
}

void	exec_pipeline_child(t_cmd *current, t_cmd *cmds,
			int prev_fd, int *pipefd)
{
	char	**cleaned_args;

	setup_pipes(prev_fd, pipefd, current);
	if (handle_redirections(current) < 0)
		exit(2);
	cleaned_args = remove_redirections(current->args);
	if (!cleaned_args)
		exit(1);
	if (is_builtin(current))
	{
		handle_builtin_in_pipeline(current, cleaned_args, cmds, cmds->envp);
		free_all_cmds(current, 1);
	}
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
