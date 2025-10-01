/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:58 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:02:58 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_exec.h"

char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	tmp = NULL;
	if (!full_path)
		return (NULL);
	return (full_path);
}

static int	check_executable(char *full_path)
{
	struct stat	st;

	if (stat(full_path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			return (0);
		if (st.st_mode & S_IXUSR)
			return (1);
		else
		{
			ft_putstr_fd(full_path, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (0);
		}
	}
	return (0);
}

char	*search_in_paths(char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (!full_path)
			return (free_paths(paths), NULL);
		if (check_executable(full_path))
			return (free_paths(paths), full_path);
		free(full_path);
		full_path = NULL;
		i++;
	}
	return (NULL);
}

char	*look_for_path(char *cmd, char **paths)
{
	char	*path;
	char	*part_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		if (!part_path)
			return (free_paths(paths), NULL);
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!path)
			return (free_paths(paths), NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (free_paths(paths), path);
		free(path);
		path = NULL;
		i++;
	}
	free_paths(paths);
	return (NULL);
}

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	paths = NULL;
}
