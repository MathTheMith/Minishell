/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:20:09 by mvachon           #+#    #+#             */
/*   Updated: 2025/06/23 16:20:09 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


#define ERR_MALLOC "malloc error\n"

char **lst_to_arr(t_list *env)
{
    int len = 0;
    t_list *tmp = env;
    char **arr;
    int i;

    if (!env)
        return NULL;

    while (tmp)
    {
        len++;
        tmp = tmp->next;
        if (tmp == env)
            break;
    }

    arr = malloc(sizeof(char *) * (len + 1));
    if (!arr)
        return NULL;

    tmp = env;
    i = 0;
    while (tmp)
    {
        arr[i++] = strdup(tmp->str);
        if (!arr[i - 1])
        {
            while (--i >= 0)
                free(arr[i]);
            free(arr);
            return NULL;
        }
        tmp = tmp->next;
        if (tmp == env)
            break;
    }
    arr[i] = NULL;
    return arr;
}

int len_list(t_list *env)
{
    int len = 0;
    t_list *tmp;

    if (!env)
        return 0;

    tmp = env;
    while (tmp)
    {
        len++;
        tmp = tmp->next;
        if (tmp == env)
            break;
    }
    return len;
}

void sort_array(char **arr, int len)
{
    int i, j;
    char *tmp;

    for (i = 0; i < len - 1; i++)
    {
        for (j = 0; j < len - i - 1; j++)
        {
            if (strcmp(arr[j], arr[j + 1]) > 0)
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

bool append(t_list **env, char *value)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_list *last;

    if (!new_node)
        return false;

    new_node->str = value;

    if (!(*env))
    {
        new_node->next = new_node;
        new_node->prev = new_node;
        *env = new_node;
        return true;
    }

    last = (*env)->prev;

    last->next = new_node;
    new_node->prev = last;
    new_node->next = *env;
    (*env)->prev = new_node;

    return true;
}

int print_error(char *msg)
{
    if (msg)
		ft_putstr_fd(msg, 2);
    return 1;
}

bool export_no_args(t_list *env)
{
    char **arr;
    int i, j;

    arr = lst_to_arr(env);
    if (!arr)
        return false;

    sort_array(arr, len_list(env));

    i = 0;
    while (arr[i])
    {
        ft_putstr_fd("export ", 1);
        j = 0;
        while (arr[i][j] && arr[i][j] != '=')
            ft_putchar_fd(arr[i][j++], 1);
        if (arr[i][j] == '=')
        {
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(&arr[i][j + 1], 1);
            ft_putstr_fd("\"\n", 1);
        }
        else
            ft_putstr_fd("\n", 1);
        free(arr[i]);
        i++;
    }
    free(arr);
    return true;
}

bool valid_identifier(char *str)
{
    int i = 0;

    if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
        return false;
	
    while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return false;
        i++;
    }
    return true;
}

int exist(char *str, t_list *env)
{
    int i = 0;
    int len = 0;
    t_list *tmp = env;

    if (!env)
        return -1;

    while (str[len] && str[len] != '=' && !(str[len] == '+' && str[len + 1] == '='))
        len++;

    while (tmp)
    {
        if (!ft_strncmp(tmp->str, str, len) && (tmp->str[len] == '\0' || tmp->str[len] == '='))
            return i;
        tmp = tmp->next;
        i++;
        if (tmp == env)
            break;
    }

    return -1;
}

char *get_var_value(char *var_name, t_list *env)
{
    t_list *tmp = env;
    int name_len = ft_strlen(var_name);

    if (!env)
        return NULL;

    while (tmp)
    {
        if (!ft_strncmp(tmp->str, var_name, name_len) && tmp->str[name_len] == '=')
            return &tmp->str[name_len + 1];
        tmp = tmp->next;
        if (tmp == env)
            break;
    }
    return NULL;
}

bool is_append_operation(char *str)
{
    int i = 0;
    
    while (str[i])
    {
        if (str[i] == '+' && str[i + 1] == '=')
            return true;
        if (str[i] == '=')
            return false;
        i++;
    }
    return false;
}

char *extract_var_name(char *str)
{
    int i = 0;
    char *name;
    
    while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
        i++;
    
    name = malloc(i + 1);
    if (!name)
        return NULL;
    
    ft_strlcpy(name, str, i + 1);
    return name;
}

char *extract_var_value(char *str)
{
    int i = 0;
    
    while (str[i] && str[i] != '=')
        i++;
    
    if (str[i] == '=')
        return ft_strdup(&str[i + 1]);
    
    return NULL;
}

bool export_var(char *str, t_list **env)
{
    int pos = exist(str, *env);
    char *new_value;
    int i;
    
    if (is_append_operation(str))
    {
        char *var_name = extract_var_name(str);
        char *append_value = extract_var_value(str);
        char *current_value;
        
        if (!var_name || !append_value)
        {
            free(var_name);
            free(append_value);
            return false;
        }
        
        current_value = get_var_value(var_name, *env);
        
        if (current_value)
        {
            int total_len = ft_strlen(var_name) + 1 + ft_strlen(current_value) + ft_strlen(append_value) + 1;
            new_value = malloc(total_len);
            if (!new_value)
            {
                free(var_name);
                free(append_value);
                return false;
            }
            ft_strlcpy(new_value, var_name, total_len);
            ft_strlcat(new_value, "=", total_len);
            ft_strlcat(new_value, current_value, total_len);
            ft_strlcat(new_value, append_value, total_len);
        }
        else
        {
            int total_len = ft_strlen(var_name) + 1 + ft_strlen(append_value) + 1;
            new_value = malloc(total_len);
            if (!new_value)
            {
                free(var_name);
                free(append_value);
                return false;
            }
            ft_strlcpy(new_value, var_name, total_len);
            ft_strlcat(new_value, "=", total_len);
            ft_strlcat(new_value, append_value, total_len);
        }
        
        free(var_name);
        free(append_value);
    }
    else
    {
        new_value = ft_strdup(str);
        if (!new_value)
            return false;
    }

    if (pos >= 0)
    {
        i = 0;
        while (i < pos)
        {
            *env = (*env)->next;
            i++;
        }
        free((*env)->str);
        (*env)->str = new_value;
    }
    else
    {
        if (!append(env, new_value))
        {
            free(new_value);
            return false;
        }
    }
    return true;
}

char *put_last_exit_code(char *str, t_cmd *cmds)
{
	int i = 0;
	int j = 0;
	char *new_str;
	char *exit_code_str;
	int exit_code_len;

	exit_code_str = ft_itoa(cmds->last_exit_code);
	if (!exit_code_str)
		return str;

	exit_code_len = ft_strlen(exit_code_str);
	new_str = malloc(ft_strlen(str) + exit_code_len);
	if (!new_str)
	{
		free(exit_code_str);
		return str;
	}

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?' && cmds->quoted[j] != 1)
		{
			ft_strlcpy(&new_str[j], exit_code_str, exit_code_len + 1);
			j += exit_code_len;
			i += 2;
		}
		else
		{
			new_str[j++] = str[i++];
		}
	}
	new_str[j] = '\0';

	free(exit_code_str);
	free(str);
	return new_str;
}

int export_input(t_cmd *cmds, t_list **env)
{
    int i = 1;
    if (!cmds->args || !cmds->args[i])
    {
        if (*env && !export_no_args(*env))
            return print_error(ERR_MALLOC);
        return 0;
    }
	cmds->args[i] = put_last_exit_code(cmds->args[i], cmds);
    while (cmds->args[i])
    {

        if (!valid_identifier(cmds->args[i]))
        {
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(cmds->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			cmds->last_exit_code = 1;

        }
        else if (!export_var(cmds->args[i], env))
        {
            return print_error(ERR_MALLOC);
        }
        i++;
    }
    return (0);
}