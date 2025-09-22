/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_exit_code.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:48 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:49 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

static int	compute_new_len(char *str, char *exit_code_str, t_cmd *cmds)
{
	int	i;
	int	len;
	int	exit_code_len;

	i = 0;
	len = 0;
	exit_code_len = ft_strlen(exit_code_str);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?' && cmds->quoted[len] != 1)
		{
			len += exit_code_len;
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len + 1);
}

static void	expand_exit_code(char *dest, char *src, char *exit_code_str,
		t_cmd *cmds)
{
	int	i;
	int	j;
	int	exit_code_len;

	i = 0;
	j = 0;
	exit_code_len = ft_strlen(exit_code_str);
	while (src[i])
	{
		if (src[i] == '$' && src[i + 1] == '?' && cmds->quoted[j] != 1)
		{
			ft_strlcpy(&dest[j], exit_code_str, exit_code_len + 1);
			j += exit_code_len;
			i += 2;
		}
		else
			dest[j++] = src[i++];
	}
	dest[j] = '\0';
}

char	*put_last_exit_code(char *str, t_cmd *cmds)
{
	char	*exit_code_str;
	char	*new_str;
	int		new_len;

	exit_code_str = ft_itoa(cmds->last_exit_code);
	if (!exit_code_str)
		return (str);
	new_len = compute_new_len(str, exit_code_str, cmds);
	new_str = malloc(new_len);
	if (!new_str)
	{
		free(exit_code_str);
		return (str);
	}
	expand_exit_code(new_str, str, exit_code_str, cmds);
	free(exit_code_str);
	free(str);
	return (new_str);
}
