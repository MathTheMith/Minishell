/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static size_t	get_var_len(const char *line, size_t start)
{
	size_t	var_len;

	var_len = 0;
	while (isalnum(line[start + var_len]) || line[start + var_len] == '_')
		var_len++;
	return (var_len);
}

static char	*expand_result(const char *line, size_t *i, t_list *env,
				char *result, size_t *ri)
{
	size_t	var_start;
	size_t	var_len;
	char	var_name[128];
	char	*val;

	var_start = *i + 1;
	var_len = get_var_len(line, var_start);
	if (var_len > 0 && var_len < sizeof(var_name))
	{
		strncpy(var_name, line + var_start, var_len);
		var_name[var_len] = '\0';
		val = get_env_value(env, var_name);
		if (val)
		{
			result = realloc(result, *ri + strlen(val)
					+ (strlen(line) - *i - var_len) + 1);
			if (!result)
				return (NULL);
			strcpy(result + *ri, val);
			*ri += strlen(val);
		}
		*i = var_start + var_len;
		return (result);
	}
	return (result);
}

char	*expand_variables(const char *line, t_list *env)
{
	char	*result;
	size_t	len;
	size_t	ri;
	size_t	i;

	len = strlen(line);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ri = 0;
	i = 0;
	while (i < len)
	{
		if (line[i] == '$' && (isalpha(line[i + 1]) || line[i + 1] == '_'))
		{
			result = expand_result(line, &i, env, result, &ri);
			if (!result)
				return (NULL);
		}
		else
			result[ri++] = line[i++];
	}
	result[ri] = '\0';
	return (result);
}
