/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:05:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:05:01 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "smart_split.h"

int	extract_env_name(const char *str, int *j, char *env_name)
{
	int	i;

	i = 0;
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		env_name[i++] = str[(*j)++];
	env_name[i] = '\0';
	return (i);
}

void	copy_env_value_to_out(char *env_val, char *out, int *k)
{
	int	i;

	i = 0;
	while (env_val && env_val[i])
		out[(*k)++] = env_val[i++];
}

static void	handle_negative_exit_code(int *temp, char *out, int *k)
{
	if (*temp < 0)
	{
		out[(*k)++] = '-';
		*temp = -*temp;
	}
}

static int	get_digit_count(int temp)
{
	int	count;

	count = 0;
	while (temp > 0)
	{
		count++;
		temp /= 10;
	}
	return (count);
}

void	copy_exit_code_to_out(int exit_code, char *out, int *k)
{
	char	exit_str[MAX_EXIT_STR];
	int		temp;
	int		digit_count;
	int		i;

	temp = exit_code;
	if (exit_code == 0)
	{
		out[(*k)++] = '0';
		return ;
	}
	handle_negative_exit_code(&temp, out, k);
	digit_count = get_digit_count(temp);
	i = 0;
	while (temp > 0)
	{
		exit_str[i++] = (temp % 10) + '0';
		temp /= 10;
	}
	i = digit_count - 1;
	while (i >= 0)
		out[(*k)++] = exit_str[i--];
}
