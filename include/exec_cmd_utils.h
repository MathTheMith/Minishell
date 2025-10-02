/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:28 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:00:28 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_CMD_UTILS_H
# define EXEC_CMD_UTILS_H

# include "minishell.h"

/* Argument utilities */
char	**remove_redirections(char **args);
int		count_args_without_redirections(char **args);

/* Builtin functions */
int		is_builtin(t_cmd *cmds);
void	exec_builtin(t_cmd *cmds, char **envp);

/* Path validation */
int		is_path_command(const char *cmd);
void	handle_path_error(t_cmd *cmds, char *cmd_name);

/* Cleanup */
void	free_all_before_exec(t_cmd *current_cmd);

#endif