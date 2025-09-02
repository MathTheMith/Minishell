/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:17 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cd_input(t_cmd *cmds)
{
    char *dir;
    char cwd[1000];
    char oldpwd[1000];
	char *resolved_dir;

	resolved_dir = NULL;
    if (!cmds->args || !cmds->args[0] || cmds->args[2])
	{
		if (cmds->args[2]) {
		    ft_putstr_fd("cd: too many arguments\n", 2);
		    cmds->last_exit_code = 1;
		    return;
		}
	}
    if (!getcwd(oldpwd, sizeof(oldpwd)))
    {
        perror("getcwd");
        cmds->last_exit_code = 1;
        return;
    }
	if (!cmds->args[1]) {
		
		dir = get_env_value(cmds->env, "HOME");
	    if (!dir) {
			ft_putstr_fd("cd: HOME not set\n", 2);
	        cmds->last_exit_code = 1;
			return;
	    }
	}
	else
		{dir = cmds->args[1];}
	if (dir[0] == '~') {
	    char *home = get_env_value(cmds->env, "HOME");
	    if (!home) {
	        ft_putstr_fd("cd: HOME not set\n", 2);
	        cmds->last_exit_code = 1;
	        return;
	    }
	    if (dir[1] == '\0')
	        dir = home;
	    else if (dir[1] == '/')
	        dir = ft_strjoin(home, dir + 1); // ~/xxx
}
	if (dir && dir[0] == '$')
    {
        resolved_dir = get_env_value(cmds->env, dir + 1); 
        if (resolved_dir)
        {
            ft_putstr_fd("cd: too many arguments\n", 2);
            cmds->last_exit_code = 1;
            return;
        }
    }
    if (chdir(dir) != 0)
    {
        perror("cd");
        cmds->last_exit_code = 1;
        return;
    }

    if (getcwd(cwd, sizeof(cwd)))
    {
        add_to_env(&cmds->env, "OLDPWD", oldpwd);
        add_to_env(&cmds->env, "PWD", cwd);
    }
    cmds->last_exit_code = 0;
}

void add_to_env(t_list **env, char *name, char *value)
{
	t_list	*tmp;
	char	*new_var;
	size_t	name_len;
	size_t	value_len;
	t_list	*new_node;

	name_len = strlen(name);
	value_len = strlen(value);

	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
		return;

	strcpy(new_var, name);
	new_var[name_len] = '=';
	strcpy(new_var + name_len + 1, value);

	tmp = *env;
	while (tmp)
	{
		if (strncmp(tmp->str, name, name_len) == 0 && tmp->str[name_len] == '=')
		{
			free(tmp->str);
			tmp->str = new_var;
			return;
		}
		tmp = tmp->next;
		if (tmp == *env)
			break;
	}

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(new_var);
		return;
	}
	new_node->str = new_var;
	new_node->next = *env;
	new_node->prev = (*env)->prev;
	(*env)->prev->next = new_node;
	(*env)->prev = new_node;
}


