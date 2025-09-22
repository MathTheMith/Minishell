/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:46 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:00:46 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_PIPELINE_H
# define EXEC_PIPELINE_H

# include "minishell.h"

/* Pipeline utilities */
int		count_commands(t_cmd *cmds);
void	setup_pipes(int prev_fd, int *pipefd, t_cmd *current);
void	cleanup_pipes(int prev_fd, int *pipefd, t_cmd *current);

/* Pipeline child process execution */
void	exec_pipeline_child(t_cmd *current, t_cmd *cmds,
			int prev_fd, int *pipefd);
void	handle_builtin_in_pipeline(t_cmd *current, char **cleaned_args,
			t_cmd *cmds, char **envp);
void	handle_external_in_pipeline(t_cmd *cmds, char **envp,
			char **cleaned_args);
/* Pipeline process management */
void	wait_pipeline_processes(pid_t *pids, int cmd_count, t_cmd *cmds);
void	handle_child_execution(t_cmd *current, t_cmd *cmds, char **envp);

void	handle_parent_cleanup(int prev_fd, int *pipefd, t_cmd *current);
int		process_single_cmd(t_cmd *current, t_cmd *cmds, char **envp,
			int *prev_fd);

#endif