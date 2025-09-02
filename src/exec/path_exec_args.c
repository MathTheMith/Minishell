/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exec_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 21:47:30 by marvin            #+#    #+#             */
/*   Updated: 2025/05/26 21:47:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_path(t_cmd *cmds, char *cmd, char **envp)
{
    char *path_env;
    char **paths;
    char *full_path;
    char *tmp;
    int i;
    struct stat st;

    if (!cmd || !*cmd)
        return (NULL);

    if (strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                ft_putstr_fd(cmd, 2);
                ft_putstr_fd(": Is a directory\n", 2);
				cmds->last_exit_code = 126;
                return (NULL);
            }
            if (st.st_mode & S_IXUSR)
            {
                return (strdup(cmd));
            }
            else
            {
                ft_putstr_fd(cmd, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return (NULL);
            }
        }
        return (NULL);
    }
    path_env = NULL;
    i = 0;
    while (envp[i])
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
        i++;
    }
    if (!path_env)
        return (NULL);

    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        if (!tmp)
        {
            free_paths(paths);
            return (NULL);
        }
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (!full_path)
        {
            free_paths(paths);
            return (NULL);
        }
        if (stat(full_path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                free(full_path);
                continue;
            }
            if (st.st_mode & S_IXUSR)
            {
                free_paths(paths);
                return (full_path);
            }
            else
            {
                ft_putstr_fd(full_path, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                free(full_path);
                continue;
            }
        }
        free(full_path);
        i++;
    }

    free_paths(paths);
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
}
char	*absolute_path(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
		{
			if (ft_strdup(cmd))
				return (ft_strdup(cmd));
			ft_putstr_fd("Error: malloc failed\n", 2);
		}
	}
	return (NULL);
}