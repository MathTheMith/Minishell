/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_exec.h"

char	*find_path(t_cmd *cmds, char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || !*cmd)
		return (NULL);
	if (strchr(cmd, '/'))
		return (handle_absolute_path(cmds, cmd));
	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(cmd, paths);
	if (!result)
		free_paths(paths);
	return (result);
}
