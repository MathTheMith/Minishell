/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:54 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:00:54 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_PROCESS_H
# define EXEC_PROCESS_H

# include "minishell.h"

/* Command execution */
t_cmd_exec	*init_cmd_exec(t_cmd *cmds, char *raw_cmd, char **envp);
void		execute_cmd(t_cmd_exec *cmd_exec, char **envp, char **args,
				t_cmd *cmd);
void		exec_child_process(t_cmd_exec *cmd_exec, char **envp,
				char **cleaned_args, t_cmd *cmd);
void		handle_exec_error(char **args, char *path);

/* Main execution */
int			check_args(t_cmd *cmds, char **envp);
void		exec_pipeline(t_cmd *cmds, char **envp);

#endif