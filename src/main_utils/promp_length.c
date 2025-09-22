/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promp_length.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:30 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:30 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*adjust_path_length(char *start, int term_width)
{
	size_t	max_path_len;

	if (term_width > 90)
		max_path_len = (size_t)(term_width - 90);
	else
		max_path_len = 10;
	if (ft_strlen(start) > max_path_len)
		return (&start[ft_strlen(start) - max_path_len]);
	return (start);
}

size_t	calculate_prompt_length(char *code, char *start)
{
	char	*blue;
	char	*reset;
	char	*suffix;
	size_t	total_len;

	blue = "\001\033[38;5;129m\002";
	reset = "\001\033[0m\002";
	suffix = "$ ";
	total_len = ft_strlen(code) + 1 + ft_strlen(blue)
		+ ft_strlen("➜  ") + ft_strlen(start) + ft_strlen(suffix)
		+ ft_strlen(reset) + 1;
	return (total_len);
}
