/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:09:03 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 22:20:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	append_char_to_line(char **line, size_t *size, size_t *capacity, char c)
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

	size = 0;
	capacity = 64;
	line = malloc(capacity);
	if (!line)
		return (NULL);
	status = ft_fill_line(&line, &size, &capacity);
	if (status <= 0)
	{
		free(line);
		return (NULL);
	}
	line[size] = '\0';
	return (line);
}
