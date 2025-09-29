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

/* String and argument utilities */
char	*get_all_cmd(t_cmd *cmds);
char	**remove_redirections(char **args);
int		count_args_without_redirections(char **args);

/* Builtin command functions */
int		is_builtin(t_cmd *cmds);
void	exec_builtin(t_cmd *cmds, char **envp);

/* Path and command validation */
int		is_path_command(const char *cmd);
void	handle_path_error(t_cmd *cmds, char *cmd_name);

#endif