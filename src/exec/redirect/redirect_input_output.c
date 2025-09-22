/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_input_output.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:10 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:10 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	handle_input_redirect(t_cmd *cmd, char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '<'\n", 2);
		cmd->last_exit_code = 2;
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmd->last_exit_code = 1;
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	is_delimiter_quoted(char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (0);
	if ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'))
		return (1);
	return (0);
}

int	handle_output_redirect(t_cmd *cmd, char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '>'\n", 2);
		cmd->last_exit_code = 2;
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(filename);
		cmd->last_exit_code = 1;
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append_redirect(t_cmd *cmd, char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '>>'\n", 2);
		cmd->last_exit_code = 2;
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(filename);
		cmd->last_exit_code = 1;
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
