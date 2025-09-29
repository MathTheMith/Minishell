#include "exec_cmd_utils.h"
#include "export.h"
#include "ft_cd.h"
#include "ft_echo.h"

int	is_builtin(t_cmd *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	if (ft_strcmp(cmds->args[0], "exit") == 0 || ft_strcmp(cmds->args[0],
			"cd") == 0 || ft_strcmp(cmds->args[0], "echo") == 0
		|| ft_strcmp(cmds->args[0], "export") == 0 || ft_strcmp(cmds->args[0],
			"unset") == 0 || ft_strcmp(cmds->args[0], "env") == 0
		|| ft_strcmp(cmds->args[0], "pwd") == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_cmd *cmds, char **envp)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return ;
	if (ft_strcmp(cmds->args[0], "exit") == 0)
	{
		if (cmds->pipe_in || cmds->pipe_out)
			exit_input_pipeline(cmds, envp);
		else
			exit_input(cmds, envp);
	}
	else if (ft_strcmp(cmds->args[0], "cd") == 0)
		cd_input(cmds);
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		echo_input(cmds);
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		export_input(cmds, &(cmds->env));
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		unset_input(cmds, &cmds->env);
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		env_input(cmds, envp);
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		pwd_input(cmds);
}

int	is_path_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '/')
		return (1);
	if (cmd[0] == '.' && cmd[1] == '/')
		return (1);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}

void	handle_path_error(t_cmd *cmds, char *cmd_name)
{
	struct stat	st;

	if (stat(cmd_name, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			cmds->last_exit_code = 126;
		else if (!(st.st_mode & S_IXUSR))
			cmds->last_exit_code = 126;
		else
		{
			cmds->last_exit_code = 126;
			ft_putstr_fd(cmd_name, 2);
			ft_putstr_fd(": cannot execute binary file\n", 2);
		}
	}
	else
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmds->last_exit_code = 127;
	}
}
