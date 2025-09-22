/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:33 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:33 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_exit_code(t_cmd *cmds)
{
	char	*code;

	if (g_interrupt == 130)
	{
		cmds->last_exit_code = 130;
		code = ft_itoa(130);
	}
	else
		code = ft_itoa(cmds->last_exit_code);
	return (code);
}

static char	*get_path_start(char *cwd)
{
	int	count;
	int	i;

	count = 0;
	i = ft_strlen(cwd) - 1;
	while (i >= 0 && count < 2)
	{
		if (cwd[i] == '/')
			count++;
		i--;
	}
	if (count == 2)
		return (&cwd[i + 2]);
	else
		return (cwd);
}

static int	get_terminal_width(void)
{
	struct winsize	w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
		return (w.ws_col);
	return (80);
}

static void	build_prompt_string(char *prompt, char *code, char *start)
{
	char	*blue;
	char	*reset;
	char	*suffix;
	size_t	pos;

	blue = "\001\033[38;5;129m\002";
	reset = "\001\033[0m\002";
	suffix = "$ ";
	pos = 0;
	ft_memcpy(prompt + pos, code, ft_strlen(code));
	pos += ft_strlen(code);
	prompt[pos++] = ' ';
	ft_memcpy(prompt + pos, blue, ft_strlen(blue));
	pos += ft_strlen(blue);
	ft_memcpy(prompt + pos, "➜  ", ft_strlen("➜  "));
	pos += ft_strlen("➜  ");
	ft_memcpy(prompt + pos, start, ft_strlen(start));
	pos += ft_strlen(start);
	ft_memcpy(prompt + pos, suffix, ft_strlen(suffix));
	pos += ft_strlen(suffix);
	ft_memcpy(prompt + pos, reset, ft_strlen(reset));
	pos += ft_strlen(reset);
	prompt[pos] = '\0';
}

char	*build_prompt(t_cmd *cmds)
{
	char	cwd[1024];
	char	*start;
	char	*code;
	char	*prompt;
	size_t	total_len;

	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_strdup("$ "));
	code = get_exit_code(cmds);
	start = get_path_start(cwd);
	start = adjust_path_length(start, get_terminal_width());
	total_len = calculate_prompt_length(code, start);
	prompt = malloc(total_len);
	if (!prompt)
	{
		free(code);
		return (NULL);
	}
	build_prompt_string(prompt, code, start);
	free(code);
	return (prompt);
}
