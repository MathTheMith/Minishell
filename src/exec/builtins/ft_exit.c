/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:13:29 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 00:18:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_exit_args(t_cmd *cmds, long long *num)
{
	if (cmds->args[1])
	{
		if (!ft_atoll_safe(cmds->args[1], num))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmds->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (2);
		}
		if (cmds->args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		return ((unsigned char)(*num % 256));
	}
	return (cmds->last_exit_code);
}

void	exit_input(t_cmd *cmds, char **envp)
{
	long long	num;
	int			exit_code;

	num = 0;
	if (!cmds || !cmds->args || !cmds->args[0])
		return ;
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 1);
	exit_code = validate_exit_args(cmds, &num);
	cleanup_and_exit_child(cmds, envp, NULL, exit_code);
}

static int	calculate_exit_code(t_cmd *cmds)
{
	long long	num;

	if (!cmds->args[1])
		return (0);
	else if (!ft_atoll_safe(cmds->args[1], &num))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmds->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	else if (cmds->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	return ((unsigned char)(num % 256));
}

void	exit_input_pipeline(t_cmd *cmds, char **envp)
{
	int	exit_code;

	exit_code = calculate_exit_code(cmds);
	if (cmds->args)
		free_string_array(cmds->args);
	if (cmds->env)
		free_env_list(cmds->env);
	if (envp)
		free_string_array(envp);
	if (cmds)
		free(cmds);
	rl_clear_history();
	exit(exit_code);
}
