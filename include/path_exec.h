/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exec.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:20 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:20 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_EXEC_H
# define PATH_EXEC_H

# include "minishell.h"

/* Path validation */
char	*handle_absolute_path(t_cmd *cmds, char *cmd);
char	*get_path_env(char **envp);
char	*absolute_path(char *cmd);

/* Path searching */
char	*search_in_paths(char *cmd, char **paths);
char	*build_full_path(char *dir, char *cmd);
char	*look_for_path(char *cmd, char **paths);

/* Main path function */
char	*find_path(t_cmd *cmds, char *cmd, char **envp);

/* Utilities */
void	free_paths(char **paths);

#endif