/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:00:00 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/07 00:00:00 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

static char	*process_heredoc_line(char *line, int quoted, t_cmd *cmd)
{
	char	*expanded;

	if (!quoted)
		expanded = expand_variables(line, cmd->env);
	else
		expanded = strdup(line);
	return (expanded);
}

static char	*append_to_buffer(char **buffer, size_t *bufsize, char *expanded)
{
	size_t	oldsize;
	char	*new_buffer;

	oldsize = *bufsize;
	*bufsize += strlen(expanded) + 1;
	new_buffer = realloc(*buffer, *bufsize + 1);
	if (!new_buffer)
		return (NULL);
	*buffer = new_buffer;
	memcpy(*buffer + oldsize, expanded, strlen(expanded));
	(*buffer)[oldsize + strlen(expanded)] = '\n';
	(*buffer)[*bufsize] = '\0';
	return (*buffer);
}

char	*read_heredoc_line(char *delimiter, t_cmd *cmd, char **buffer,
			size_t *bufsize)
{
	char	*line;
	size_t	len;
	ssize_t	nread;
	int		quoted;
	char	*expanded;

	line = NULL;
	len = 0;
	quoted = is_delimiter_quoted(delimiter);
	while (1)
	{
		write(1, "> ", 2);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			return (NULL);
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		expanded = process_heredoc_line(line, quoted, cmd);
		if (!expanded || !append_to_buffer(buffer, bufsize, expanded))
		{
			free(expanded);
			free(line);
			return (NULL);
		}
		free(expanded);
	}
	free(line);
	return (*buffer);
}

int	setup_heredoc_pipe(char *buffer, size_t bufsize, t_cmd *cmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		cmd->last_exit_code = 1;
		return (-1);
	}
	if (buffer)
		write(pipefd[1], buffer, bufsize);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

int	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	char			*heredoc_buffer;
	size_t			heredoc_bufsize;
	void			(*old_sigint)(int);

	heredoc_buffer = NULL;
	heredoc_bufsize = 0;
	old_sigint = signal(SIGINT, SIG_DFL);
	if (!read_heredoc_line(delimiter, cmd, &heredoc_buffer, &heredoc_bufsize))
	{
		free(heredoc_buffer);
		signal(SIGINT, old_sigint);
		cmd->last_exit_code = 130;
		return (-1);
	}
	signal(SIGINT, old_sigint);
	if (setup_heredoc_pipe(heredoc_buffer, heredoc_bufsize, cmd) < 0)
	{
		free(heredoc_buffer);
		return (-1);
	}
	free(heredoc_buffer);
	return (1);
}
