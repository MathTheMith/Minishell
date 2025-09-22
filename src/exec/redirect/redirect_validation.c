/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_validation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:03:17 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:03:17 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirections.h"

int	check_redirect_token(char **args, int i)
{
	if (!args[i + 1] || args[i + 1][0] == '\0')
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n", 57);
		return (-1);
	}
	if (ft_strcmp(args[i + 1], ">") == 0 || ft_strcmp(args[i + 1], ">>") == 0
		|| ft_strcmp(args[i + 1], "<") == 0
		|| ft_strcmp(args[i + 1], "<<") == 0)
	{
		write_syntax_error(args[i + 1]);
		return (-1);
	}
	return (0);
}

static void	write_redirect_error(int redirect_count, int lr_count)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	if (lr_count >= 0)
	{
		if (redirect_count == 3)
			write(2, ">", 1);
		else
			write(2, ">>", 2);
	}
	else
	{
		if (redirect_count == -3)
			write(2, "<", 1);
		else
			write(2, "<<", 2);
	}
	write(2, "'\n", 2);
}

static void	count_redirects(char *str, int *redirect_count, int *lr_count)
{
	int	j;

	*redirect_count = 0;
	*lr_count = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] == '>')
		{
			(*redirect_count)++;
			(*lr_count)++;
		}
		else if (str[j] == '<')
		{
			(*redirect_count)--;
			(*lr_count)--;
		}
		j++;
	}
}

int	check_multiple_redirects(char *str)
{
	int	redirect_count;
	int	lr_count;

	count_redirects(str, &redirect_count, &lr_count);
	if (redirect_count >= 3 || redirect_count <= -3)
	{
		write_redirect_error(redirect_count, lr_count);
		return (-1);
	}
	return (0);
}

int	bad_redirection(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
			|| ft_strcmp(args[i], "<") == 0
			|| ft_strcmp(args[i], "<<") == 0)
		{
			if (check_redirect_token(args, i) < 0)
				return (-1);
			i += 2;
		}
		else
		{
			if (check_multiple_redirects(args[i]) < 0)
				return (-1);
			i++;
		}
	}
	return (0);
}
