/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:02:10 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/12 00:50:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_echo.h"

static int	skip_n_options(char **args, int *i)
{
	int	newline;

	newline = 1;
	while (args[*i] && (ft_strcmp(args[*i], "-n") == 0
			|| check_big_n(args[*i])))
	{
		(*i)++;
		newline = 0;
	}
	return (newline);
}

static int	skip_redirections(char **args, int *i)
{
	if ((ft_strcmp(args[*i], ">") == 0 || ft_strcmp(args[*i], ">>") == 0
			|| ft_strcmp(args[*i], "<") == 0 || ft_strcmp(args[*i], "<<") == 0)
		&& args[*i + 1])
	{
		*i += 2;
		return (1);
	}
	return (0);
}

static void	handle_exit_code_expansion(t_cmd *cmds, char *arg, int *j,
		int quoted)
{
	if (arg[*j] == '$' && arg[*j + 1] == '?' && quoted != 1)
	{
		if (g_interrupt == 130)
		{
			cmds->last_exit_code = 130;
		}
		ft_putnbr_fd(cmds->last_exit_code, 1);
		(*j)++;
	}
	else
	{
		ft_putchar_fd(arg[*j], 1);
		cmds->last_exit_code = 0;
	}
}

static void	print_argument(t_cmd *cmds, int arg_index)
{
	int		j;
	char	*arg;
	int		quoted;

	j = 0;
	arg = cmds->args[arg_index];
	quoted = cmds->quoted[arg_index];
	while (arg[j])
	{
		handle_exit_code_expansion(cmds, arg, &j, quoted);
		j++;
	}
}

void	echo_input(t_cmd *cmds)
{
	int	i;
	int	newline;

	i = 1;
	if (ft_strcmp(cmds->args[0], "echo") != 0)
		return ;
	newline = skip_n_options(cmds->args, &i);
	while (cmds->args[i])
	{
		if (skip_redirections(cmds->args, &i))
			continue ;
		print_argument(cmds, i);
		i++;
		if (cmds->args[i])
			ft_putchar_fd(' ', 1);
	}
	if (newline)
		ft_putchar_fd('\n', 1);
}
