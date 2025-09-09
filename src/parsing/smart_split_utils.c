/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 04:51:11 by marvin            #+#    #+#             */
/*   Updated: 2025/09/02 11:39:49 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Parse une chaîne en gérant les quotes et les variables d'environnement
void	parse_input_string(t_cmd *cmd, char *src, char *dest)
{
    int	i = 0;
    int	j = 0;
    int	src_len = ft_strlen(src);

    while (i < src_len)
    {
        if (src[i] == '\'')
            handle_single_quotes(src, dest, &i, &j);
        else if (src[i] == '\"')
            handle_double_quotes(cmd, src, dest, &i, &j);
        else if (src[i] == '$' && ft_strcmp(src, "cd"))
        {
            handle_env_variable(cmd, src, dest, &i, &j);
        }
        else
            dest[j++] = src[i++];
    }
    dest[j] = '\0';
}

// Ignore les espaces à partir de l'index courant
void	skip_spaces(const char *str, int *i)
{
    while (str[*i] && str[*i] == ' ')
        (*i)++;
}

// Détecte et gère l'ouverture/fermeture de quotes
void	update_quote_state(const char c, int *in_quotes, char *quote)
{
    if (!*in_quotes && (c == '\'' || c == '\"'))
    {
        *in_quotes = 1;
        *quote = c;
    }
    else if (*in_quotes && c == *quote)
    {
        *in_quotes = 0;
        *quote = 0;
    }
}

// Compte le nombre d'arguments dans une chaîne, en tenant compte des quotes
int	count_string_args(const char *str)
{
    int	count = 0;
    int	i = 0;
    int	in_quotes = 0;
    char	quote = 0;

    while (str[i])
    {
        skip_spaces(str, &i);
        if (!str[i])
            break;
        count++;
        while (str[i] && (str[i] != ' ' || in_quotes))
        {
            update_quote_state(str[i], &in_quotes, &quote);
            i++;
        }
    }
    return count;
}

// Calcule la longueur d'un argument à partir de la position courante
int	get_arg_length(const char *str, int start, int *quote_type)
{
    int		i = start;
    int		len = 0;
    int		in_quotes = 0;
    char	quote = 0;

    while (str[i] && (str[i] != ' ' || in_quotes))
    {
        if (!in_quotes && (str[i] == '\'' || str[i] == '\"'))
        {
            if (quote_type)
            {
                if (str[i] == '\'')
                    *quote_type = 1;
                else
                    *quote_type = 2;
            }
            in_quotes = 1;
            quote = str[i];
        }
        else if (in_quotes && str[i] == quote)
        {
            in_quotes = 0;
            quote = 0;
        }
        i++;
        len++;
    }
    return len;
}

// Alloue et extrait la sous-chaîne brute de l'argument
char	*extract_raw_arg(const char *str, int start, int len)
{
    char *raw_arg = malloc(len + 1);
    if (!raw_arg)
        return NULL;
    ft_strlcpy(raw_arg, str + start, len + 1);
    return raw_arg;
}

// Extrait et parse un argument de la chaîne, gère les quotes et l'expansion
char	*extract_and_parse_arg(t_cmd *cmd, char *str, int *pos, int arg_quoted)
{
    int		start;
    int		len;
    char	*raw_arg;
    char	*parsed_arg;
    char	*final_arg;
    int		quote_type = 0;

    skip_spaces(str, pos);
    start = *pos;
    len = get_arg_length(str, start, &quote_type);
    if (cmd && cmd->quoted)
        cmd->quoted[arg_quoted] = quote_type;
    *pos += len;

    raw_arg = extract_raw_arg(str, start, len);
    if (!raw_arg)
        return NULL;

    parsed_arg = malloc(len * 2 + 512);
    if (!parsed_arg)
    {
        free(raw_arg);
        return NULL;
    }
    parse_input_string(cmd, raw_arg, parsed_arg);
    free(raw_arg);

    final_arg = ft_strdup(parsed_arg);
    free(parsed_arg);
    return final_arg;
}
