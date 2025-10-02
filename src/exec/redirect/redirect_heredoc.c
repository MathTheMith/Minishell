/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:04:23 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 23:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	g_interrupt = 130;
	write(1, "\n", 1);
}

char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(delimiter);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] != '\'' && delimiter[i] != '"')
			result[j++] = delimiter[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

static char	*process_heredoc_line(char *line, int quoted, t_cmd *cmd)
{
	char	*expanded;

	if (!quoted)
		expanded = expand_variables(line, cmd->env);
	else
		expanded = ft_strdup(line);
	return (expanded);
}

static int	append_to_buffer(char **buffer, size_t *size, char *expanded)
{
	char	*new_buffer;
	size_t	exp_len;
	size_t	new_size;

	if (!expanded)
		return (1);
	exp_len = ft_strlen(expanded);
	new_size = *size + exp_len + 1;
	new_buffer = malloc(new_size + 1);
	if (!new_buffer)
		return (0);
	if (*buffer && *size > 0)
		ft_memcpy(new_buffer, *buffer, *size);
	ft_memcpy(new_buffer + *size, expanded, exp_len);
	new_buffer[*size + exp_len] = '\n';
	new_buffer[new_size] = '\0';
	free(*buffer);
	*buffer = new_buffer;
	*size = new_size;
	return (1);
}

int	read_heredoc_loop(char *clean_delim, t_cmd *cmd,
			char **buffer, size_t *bufsize)
{
	char	*line;
	int		quoted;
	char	*expanded;
	void	(*old_handler)(int);
	void	(*old_quit)(int);

	quoted = is_delimiter_quoted(clean_delim);
	old_handler = signal(SIGINT, heredoc_sigint_handler);
	old_quit = signal(SIGQUIT, SIG_IGN);
	g_interrupt = 0;
	while (1)
	{
		line = ft_readline();
		if (g_interrupt == 130)
		{
			signal(SIGINT, old_handler);
			signal(SIGQUIT, old_quit);
			free(line);
			g_interrupt = 0;
			return (0);
		}
		if (!line)
		{
			signal(SIGINT, old_handler);
			signal(SIGQUIT, old_quit);
			write(2, "minishell: warning: here-document delimited", 43);
			write(2, " by end-of-file (wanted `", 25);
			write(2, clean_delim, ft_strlen(clean_delim));
			write(2, "')\n", 3);
			return (1);
		}
		if (ft_strcmp(line, clean_delim) == 0)
		{
			free(line);
			break ;
		}
		expanded = process_heredoc_line(line, quoted, cmd);
		free(line);
		if (!append_to_buffer(buffer, bufsize, expanded))
		{
			free(expanded);
			signal(SIGINT, old_handler);
			signal(SIGQUIT, old_quit);
			return (0);
		}
		free(expanded);
	}
	signal(SIGINT, old_handler);
	signal(SIGQUIT, old_quit);
	return (1);
}