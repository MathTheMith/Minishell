/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:31 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool remove_env_var(char *name, t_list **env)
{
    t_list *tmp;
    t_list *to_delete;
    int name_len = 0;

    (void)to_delete;
    if (!*env || !name)
        return false;

    while (name[name_len])
        name_len++;

    tmp = *env;
    
    while (tmp)
    {
        if (!ft_strncmp(tmp->str, name, name_len) && 
            (tmp->str[name_len] == '\0' || tmp->str[name_len] == '='))
        {
            to_delete = tmp;
            
            if (tmp->next == tmp && tmp->prev == tmp)
            {
                free(tmp->str);
                free(tmp);
                *env = NULL;
                return true;
            }
            
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            
            if (*env == tmp)
                *env = tmp->next;
            
            free(tmp->str);
            free(tmp);
            return true;
        }
        
        tmp = tmp->next;
        
        if (tmp == *env)
            break;
    }
    
    return false;
}

void unset_input(t_cmd *cmds, t_list **env)
{
    int i = 1;
    
    if (ft_strcmp(cmds->args[0], "unset") != 0)
        return;
    
    if (!cmds->args[1])
    {
        cmds->last_exit_code = 0;
        return;
    }
    
    while (cmds->args[i])
    {
            remove_env_var(cmds->args[i], env);
            
            unsetenv(cmds->args[i]);
        i++;
    }
    
    cmds->last_exit_code = 0;
}
