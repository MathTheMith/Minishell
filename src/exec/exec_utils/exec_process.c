/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:03:47 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 15:03:47 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_process.h"
#include "exec_cmd_utils.h"
#include "path_exec.h"
#include "redirections.h"

t_cmd_exec	*init_cmd_exec(t_cmd *cmds, char *raw_cmd, char **envp)
{
	t_cmd_exec	*cmd;

	cmd = malloc(sizeof(t_cmd_exec));
	if (!cmd)
		return (NULL);
	cmd->raw_cmd = raw_cmd;
	cmd->args = NULL;
	cmd->path = find_path(cmds, raw_cmd, envp);
	if (!cmd->path)
	{
		free(cmd);
		ft_putstr_fd(cmds->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cmds->last_exit_code = 127;
		return (NULL);
	}
	return (cmd);
}

void	handle_exec_error(char **args, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		else if (!(st.st_mode & S_IXUSR))
		{
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void free_all_before_exec(t_cmd *current_cmd)
{
    t_cmd *temp = current_cmd;
    
    while (temp) {
        free_string_array(temp->args);
        if (temp->quoted)
            free(temp->quoted);
        if (temp->infile)
            free(temp->infile);
        if (temp->outfile)
            free(temp->outfile);
        temp = temp->next;
    }
}

void	exec_child_process(t_cmd_exec *cmd_exec, char **envp,
			char **cleaned_args, t_cmd *cmd)
{
	setup_child_signals();
	if (handle_redirections(cmd) < 0)
		exit(2);
	execve(cmd_exec->path, cleaned_args, envp);
	handle_exec_error(cleaned_args, cmd_exec->path);
}

pid_t	fork_and_handle(t_cmd_exec *cmd_exec, char **envp, char **cleaned_args,
							t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		cmd->last_exit_code = 1;
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, sigquit_handler);
		exec_child_process(cmd_exec, envp, cleaned_args, cmd);
	}
	else
		cmd->last_exit_code = wait_for_child(pid);
	return (pid);
}

void	execute_cmd(t_cmd_exec *cmd_exec, char **envp, char **args, t_cmd *cmd)
{
	char	**cleaned_args;

	if (!cmd_exec || !args || !cmd_exec->path)
	{
		ft_putstr_fd("Error: Invalid command\n", 2);
		cmd->last_exit_code = 127;
		return ;
	}
	cleaned_args = remove_redirections(cmd->args);
	if (!cleaned_args)
	{
		cmd->last_exit_code = 1;
		return ;
	}
	if (fork_and_handle(cmd_exec, envp, cleaned_args, cmd) == -1)
	{
		free_cleaned_args(cleaned_args);
		return ;
	}
	free_cleaned_args(cleaned_args);
}
