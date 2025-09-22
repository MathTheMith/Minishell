/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:05 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:05 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"
#include "ft_echo.h"

static size_t	get_var_len(const char *line, size_t start)
{
	size_t	var_len;

	var_len = 0;
	while (isalnum(line[start + var_len]) || line[start + var_len] == '_')
		var_len++;
	return (var_len);
}

static char	*append_var_value(t_expand *expand, const char *val)
{
	expand->result = ft_realloc(expand->result, *expand->ri, *expand->ri
			+ ft_strlen(val) + (ft_strlen(expand->line) - *expand->i
				- get_var_len(expand->line, *expand->i + 1)) + 1);
	if (!expand->result)
		return (NULL);
	ft_strcpy(expand->result + *expand->ri, val);
	*expand->ri += ft_strlen(val);
	return (expand->result);
}

char	*expand_result(t_expand *expand)
{
	size_t	var_start;
	size_t	var_len;
	char	var_name[128];
	char	*val;

	var_start = *expand->i + 1;
	var_len = get_var_len(expand->line, var_start);
	if (var_len > 0 && var_len < sizeof(var_name))
	{
		ft_strncpy(var_name, expand->line + var_start, var_len);
		var_name[var_len] = '\0';
		val = get_env_value(expand->env, var_name);
		if (val)
		{
			expand->result = append_var_value(expand, val);
			if (!expand->result)
				return (NULL);
		}
		*expand->i = var_start + var_len;
		return (expand->result);
	}
	return (expand->result);
}

static char	*process_expansion(t_expand *expand)
{
	size_t	len;

	len = ft_strlen(expand->line);
	while (*expand->i < len)
	{
		if (expand->line[*expand->i] == '$'
			&& (isalpha(expand->line[*expand->i + 1])
				|| expand->line[*expand->i + 1] == '_'))
		{
			expand->result = expand_result(expand);
			if (!expand->result)
				return (NULL);
		}
		else
			expand->result[(*expand->ri)++] = expand->line[(*expand->i)++];
	}
	expand->result[*expand->ri] = '\0';
	return (expand->result);
}

char	*expand_variables(const char *line, t_list *env)
{
	char		*result;
	size_t		len;
	size_t		ri;
	size_t		i;
	t_expand	expand;

	len = ft_strlen(line);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ri = 0;
	i = 0;
	expand.line = line;
	expand.i = &i;
	expand.result = result;
	expand.ri = &ri;
	expand.env = env;
	return (process_expansion(&expand));
}
