/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:19 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_big_n(char *str)
{
	int i = 0;

	if (str[i] != '-')
		return 0;
	i++;
	if (str[i] != 'n')
		return 0;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return 1;
	return 0;
}

int check_echo_args(t_cmd *cmds)
{
    int i = 1;
    int found_n = 0;
    
    while (cmds->args[i] && (ft_strcmp(cmds->args[i], "-n") == 0 || check_big_n(cmds->args[i]) == 1))
    {
        found_n = 1;
        i++;
    }
    if (found_n)
		return i;
	else
		return 1;
}

void echo_input(t_cmd *cmds)
{
    int i = 1;
    int newline = 1;

    if (ft_strcmp(cmds->args[0], "echo") != 0)
        return;

    // Vérifier les options -n, -nnn, etc.
    while (cmds->args[i] && (ft_strcmp(cmds->args[i], "-n") == 0 || check_big_n(cmds->args[i])))
        i++, newline = 0;

    while (cmds->args[i])
    {
        // Ignorer les redirections simples >, >>, <, << si suivies d’un fichier
        if ((ft_strcmp(cmds->args[i], ">") == 0 || 
             ft_strcmp(cmds->args[i], ">>") == 0 || 
             ft_strcmp(cmds->args[i], "<") == 0 || 
             ft_strcmp(cmds->args[i], "<<") == 0) && cmds->args[i + 1])
        {
            i += 2;
            continue;
        }

        // Parcourir chaque caractère
        int j = 0;
        while (cmds->args[i][j])
        {
            // Remplacer $? par le dernier exit code
            if (cmds->args[i][j] == '$' && cmds->args[i][j + 1] == '?' && cmds->quoted[i] != 1)
            {
                if (g_interrupt == 130)
                {
                    cmds->last_exit_code = 130;
                    g_interrupt = 0;
                }
                ft_putnbr_fd(cmds->last_exit_code, 1);
                j += 1;
            }
            else
			{
                ft_putchar_fd(cmds->args[i][j], 1); 
				cmds->last_exit_code = 0;
			}
            j++;
        }

        i++;
        if (cmds->args[i])
            ft_putchar_fd(' ', 1);
    }

    if (newline)
        ft_putchar_fd('\n', 1);
}



char *get_env_value(t_list *env, const char *name)
{
    size_t len = strlen(name);
    t_list *tmp = env;
    if (!tmp)
        return NULL;

    while (tmp)
    {
        if (ft_strncmp(tmp->str, name, len) == 0 && tmp->str[len] == '=')
            return tmp->str + len + 1;
        tmp = tmp->next;
        if (tmp == env)
            break;
    }

    return NULL;
}

