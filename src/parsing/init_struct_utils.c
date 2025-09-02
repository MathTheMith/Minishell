/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:19:48 by tfournie          #+#    #+#             */
/*   Updated: 2025/08/28 00:04:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../../libft/libft.h"

// Ces fonctions restent inchangées car elles ne sont pas appelées depuis init_cmd
char *remove_tabs_outside_quotes(char *input)
{
    int len = strlen(input);
    int i = 0, j = 0;
    char *result;
    int in_quotes = 0;
    char quote_char = 0;

    result = malloc(len + 1);
    if (!result)
        return (NULL);

    while (input[i])
    {
        if (!in_quotes && (input[i] == '\'' || input[i] == '\"'))
        {
            in_quotes = 1;
            quote_char = input[i];
            result[j++] = input[i++];
        }
        else if (in_quotes && input[i] == quote_char)
        {
            in_quotes = 0;
            quote_char = 0;
            result[j++] = input[i++];
        }
        else if (!in_quotes && input[i] == '\t')
        {
            i++;
            continue;
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return (result);
}

char *add_spaces_around_redirections(char *input)
{
    int i = 0, j = 0;
    int in_quotes = 0;
    char quote_char = 0;
    char *result = malloc(strlen(input) * 2 + 1);
    if (!result)
        return NULL;

    while (input[i])
    {
        if ((input[i] == '\'' || input[i] == '\"') && !in_quotes)
        {
            in_quotes = 1;
            quote_char = input[i];
            result[j++] = input[i++];
            continue;
        }
        else if (in_quotes && input[i] == quote_char)
        {
            in_quotes = 0;
            quote_char = 0;
            result[j++] = input[i++];
            continue;
        }

        if (!in_quotes && (input[i] == '>' || input[i] == '<'))
        {
            char type = input[i];

            // Vérifier les combinaisons interdites comme <> ou ><
            if ((input[i] == '>' && input[i + 1] == '<') ||
                (input[i] == '<' && input[i + 1] == '>'))
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
                ft_putchar_fd(input[i], 2);
                ft_putchar_fd(input[i + 1], 2);
                ft_putstr_fd("'\n", 2);
                free(result);
                return NULL;
            }

            int count = 1;
            while (input[i + count] == type)
                count++;
            
            if (count > 2)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
                if (type == '>')
                    ft_putstr_fd(">>", 2);
                else
                    ft_putstr_fd("<<", 2);
                ft_putstr_fd("'\n", 2);
                free(result);
                return NULL;
            }

            char next_after_redirections = input[i + count];
            if (next_after_redirections && (next_after_redirections == '>' || next_after_redirections == '<') 
                && next_after_redirections != type)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
                ft_putchar_fd(next_after_redirections, 2);
                ft_putstr_fd("'\n", 2);
                free(result);
                return NULL;
            }

            if (j > 0 && result[j - 1] != ' ')
                result[j++] = ' ';

            for (int k = 0; k < count; k++)
                result[j++] = type;

            i += count;

            if (input[i] && input[i] != ' ')
                result[j++] = ' ';
        }
        else
        {
            result[j++] = input[i++];
        }
    }

    result[j] = '\0';
    return result;
}

int count_cmd(t_data *data)
{
    int i;
    int nb_cmd;
    int in_single_quote;
    int in_double_quote;
    
    i = 0;
    nb_cmd = 1;
    in_single_quote = 0;
    in_double_quote = 0;
    
    while (data->input[i])
    {
        // Gestion des guillemets simples
        if (data->input[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
        }
        // Gestion des guillemets doubles
        else if (data->input[i] == '"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
        }
        // Compter les pipes seulement si on n'est pas dans des guillemets
        else if (data->input[i] == '|' && !in_single_quote && !in_double_quote)
        {
            nb_cmd++;
        }
        i++;
    }
    return (nb_cmd);
}

// FONCTIONS MODIFIÉES : ajout du paramètre processed
int count_args(char *processed)
{
	char **split;
	int count = 0;
	
	// Utilise directement la chaîne déjà traitée
	split = ft_split(processed, ' ');
	if (!split)
		return (0);
	
	while (split[count])
		count++;
	
	for (int i = 0; split[i]; i++)
		free(split[i]);
	free(split);
	
	return (count);
}

char *check_infile(char *processed)
{
	char **cmd_line;
	char *result = NULL;
	int j = 0;
	
	// Utilise directement la chaîne déjà traitée
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (NULL);
	
	while (cmd_line[j])
	{
		if ((ft_strcmp(cmd_line[j], "<") == 0 || ft_strcmp(cmd_line[j], "<<") == 0) && cmd_line[j + 1])
		{
			result = ft_strdup(cmd_line[j + 1]);
			break;
		}
		j++;
	}
	
	for (int k = 0; cmd_line[k]; k++)
		free(cmd_line[k]);
	free(cmd_line);
	
	return (result);
}

int check_ascending(char *processed)
{
	char **cmd_line;
	int result = 0;
	int j = 0;
	
	// Utilise directement la chaîne déjà traitée
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (0);
		
	while (cmd_line[j])
	{
		if (ft_strcmp(cmd_line[j], "<<") == 0)
		{
			result = 2;
		}
		else if (ft_strcmp(cmd_line[j], "<") == 0)
		{
			result = 1;
		}
		j++;
	}

	for (int k = 0; cmd_line[k]; k++)
		free(cmd_line[k]);
	free(cmd_line);
	
	return (result);
}

char *check_outfile(char *processed)
{
	char **cmd_line;
	char *result = NULL;
	int j = 0;
	
	// Utilise directement la chaîne déjà traitée
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (NULL);
	
	while (cmd_line[j])
	{
		if ((ft_strcmp(cmd_line[j], ">") == 0 || ft_strcmp(cmd_line[j], ">>") == 0) && cmd_line[j + 1])
		{
			result = ft_strdup(cmd_line[j + 1]);
			break;
		}
		j++;
	}
	
	for (int k = 0; cmd_line[k]; k++)
		free(cmd_line[k]);
	free(cmd_line);
	
	return (result);
}

int check_descending(char *processed)
{
	char **cmd_line;
	int result = 0;
	int j = 0;
	
	// Utilise directement la chaîne déjà traitée
	cmd_line = ft_split(processed, ' ');
	if (!cmd_line)
		return (0);
	
	while (cmd_line[j])
	{
		if (ft_strcmp(cmd_line[j], ">>") == 0)
		{
			result = 2;
			break;
		}
		else if (ft_strcmp(cmd_line[j], ">") == 0)
		{
			result = 1;
		}
		j++;
	}
	
	for (int k = 0; cmd_line[k]; k++)
		free(cmd_line[k]);
	free(cmd_line);
	
	return (result);
}