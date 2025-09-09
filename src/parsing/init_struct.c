/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:20:37 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/02 12:19:22 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../../libft/libft.h"

bool check_pipe_in(char **cmd_pipe, int i)
{
	if (cmd_pipe[i - 1] != NULL && i != 0)
		return(true);
	return (false);
}

bool check_pipe_out(char **cmd_pipe, int i)
{
	if (cmd_pipe[i + 1] != NULL)
		return(true);
	return (false);
}

void init_cmd(t_cmd **command, t_data *data, int i, char **cmd_pipe, t_list *env_list)
{
	char *processed_cmd;

	processed_cmd = add_spaces_around_redirections(cmd_pipe[i]);
	if (!processed_cmd)
	{
		command[i] = NULL;
		return;
	}
	
	command[i] = malloc(sizeof(t_cmd));
    if (command[i] == NULL)
	{
		free(processed_cmd);
		return;
	}
	
	command[i]->data = data;
	command[i]->id = i;
	command[i]->env = env_list;
	command[i]->nb_args = count_args(processed_cmd);
	command[i]->quoted = malloc(sizeof(int) * command[i]->nb_args);
	memset(command[i]->quoted, 0, sizeof(int) * command[i]->nb_args);
	command[i]->args = manage_args(command[i], cmd_pipe[i]);
	command[i]->infile = check_infile(processed_cmd);
	command[i]->ascending = check_ascending(processed_cmd);
	command[i]->outfile = check_outfile(processed_cmd);
	command[i]->descending = check_descending(processed_cmd);
	command[i]->pipe_in = check_pipe_in(cmd_pipe, i);
	command[i]->pipe_out = check_pipe_out(cmd_pipe, i);
	command[i]->next = NULL;
	
	free(processed_cmd);
}

static char **split_by_pipes(char *input)
{
    char **result;
    int pipe_count = 0;
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;
    
    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (input[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (input[i] == '|' && !in_single_quote && !in_double_quote)
            pipe_count++;
        i++;
    }
    
    result = malloc((pipe_count + 2) * sizeof(char *));
    if (!result)
        return (NULL);
    
    int cmd_index = 0;
    int start = 0;
    i = 0;
    in_single_quote = 0;
    in_double_quote = 0;
    
    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (input[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (input[i] == '|' && !in_single_quote && !in_double_quote)
        {
            int len = i - start;
            result[cmd_index] = malloc(len + 1);
            if (!result[cmd_index])
            {
                for (int j = 0; j < cmd_index; j++)
                    free(result[j]);
                free(result);
                return (NULL);
            }
            ft_strlcpy(result[cmd_index], input + start, len + 1);
            cmd_index++;
            start = i + 1;
        }
        i++;
    }
    
    int len = i - start;
    result[cmd_index] = malloc(len + 1);
    if (!result[cmd_index])
    {
        for (int j = 0; j < cmd_index; j++)
            free(result[j]);
        free(result);
        return (NULL);
    }
    ft_strlcpy(result[cmd_index], input + start, len + 1);
    result[cmd_index + 1] = NULL;
    
    return (result);
}

void init_command(t_cmd ***command, t_data *data, t_list *env_list)
{
    int i;
    char **cmd_pipe;
    
    i = 0;
    data->nb_cmd = count_cmd(data);
    *command = malloc((data->nb_cmd + 1) * sizeof(t_cmd *));
    if (*command == NULL)
        return ;
    
    if (data->input[0] == '|')
    {
        free(data);
        free(*command);
        return ;
    }
    else
        cmd_pipe = split_by_pipes(data->input);
    
    while (i < data->nb_cmd)
    {
        init_cmd(*command, data, i, cmd_pipe, env_list);
        if ((*command)[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                if ((*command)[j])
                {
                    if ((*command)[j]->args)
                        free((*command)[j]->args);
                    if ((*command)[j]->quoted)
                        free((*command)[j]->quoted);
                    if ((*command)[j]->infile)
                        free((*command)[j]->infile);
                    if ((*command)[j]->outfile)
                        free((*command)[j]->outfile);
                    free((*command)[j]);
                }
            }
            free(*command);
            *command = NULL;
            
            for (int k = 0; cmd_pipe[k]; k++)
                free(cmd_pipe[k]);
            free(cmd_pipe);
            return;
        }
        i++;
    }
    (*command)[data->nb_cmd] = NULL;
    
    i = 0;
    while (i < data->nb_cmd - 1)
    {
        if ((*command)[i]->pipe_out == true)
            (*command)[i]->next = (*command)[i + 1];
        i++;
    }
    
    for (i = 0; cmd_pipe[i]; i++)
        free(cmd_pipe[i]);
    free(cmd_pipe);
}