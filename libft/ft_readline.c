/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:09:03 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 04:12:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	disable_bracketed_paste(void)
{
	write(1, "\033[?2004l", 8);
}

static void	enable_bracketed_paste(void)
{
	write(1, "\033[?2004h", 8);
}

static int	append_char_to_line(char **line, size_t *size,
				size_t *capacity, char c)
{
	char	*new_line;

	if (*size + 1 >= *capacity)
	{
		new_line = ft_realloc(*line, *capacity, *capacity * 2);
		if (!new_line)
		{
			free(*line);
			*line = NULL;
			return (0);
		}
		*line = new_line;
		*capacity *= 2;
	}
	(*line)[(*size)++] = c;
	return (1);
}

static int	ft_fill_line(char **line, size_t *size, size_t *capacity)
{
	char	c;
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, &c, 1);
	while (bytes_read > 0)
	{
		if (c == '\n')
			break ;
		if (!append_char_to_line(line, size, capacity, c))
			return (-1);
		bytes_read = read(STDIN_FILENO, &c, 1);
	}
	if (bytes_read <= 0 && *size == 0)
		return (0);
	return (1);
}

char	*ft_readline(void)
{
	char	*line;
	size_t	size;
	size_t	capacity;
	int		status;

	if (g_interrupt == 130)
		return (NULL);
	disable_bracketed_paste();
	write(STDOUT_FILENO, "> ", 2);
	size = 0;
	capacity = 64;
	line = malloc(capacity);
	if (!line)
	{
		enable_bracketed_paste();
		return (NULL);
	}
	status = ft_fill_line(&line, &size, &capacity);
	enable_bracketed_paste();
	if (status <= 0 || g_interrupt == 130)
	{
		free(line);
		return (NULL);
	}
	line[size] = '\0';
	return (line);
}
