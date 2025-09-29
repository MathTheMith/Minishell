#include "minishell.h"

void	cleanup_and_exit(t_cmd *cmds, int exit_code)
{
	if (cmds)
	{
		if (cmds->env)
		{
			free_env_list(cmds->env);
		}
		free_all_cmds(cmds, 1);
	}
	rl_clear_history();
	exit(exit_code);
}

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
	if (envp)
		free_string_array(envp);
	cleanup_and_exit(cmds, exit_code);
}

void	cleanup_child_process(char **envp, t_cmd *cmds, int exit_code)
{
	if (envp)
		free_string_array(envp);
	if (cmds->env)
		free_env_list(cmds->env);
	if (cmds->pids)
		free(cmds->pids);
	if (cmds->command_array)
		free(cmds->command_array);
	if (cmds)
		free_all_cmds(cmds, 1);
	rl_clear_history();
	exit(exit_code);
}


void	exit_input_pipeline(t_cmd *cmds, char **envp)
{
	long long	num;
	if (!cmds->args[1])
	{
		cleanup_child_process(envp, cmds, 0);
	}
	if (!ft_atoll_safe(cmds->args[1], &num))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmds->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_child_process(envp, cmds, 2);
	}
	if (cmds->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cleanup_child_process(envp, cmds, 1);
	}
	cleanup_child_process(envp, cmds, (unsigned char)(num % 256));
}
