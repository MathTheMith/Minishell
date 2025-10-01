

#include "minishell.h"
#include "check_invalid.h"
#include "exec_process.h"

int	check_info(t_cmd *cmds, t_cmd **command, char **envp)
{
	t_cmd	*cur;

	if (cmds)
		cmds->command_array = command;
	cur = cmds;
	while (cur)
	{
		if (cur->args && cur->args[0])
		{
			if (check_invalid_start(cur->args[0], 0) == EXIT_FAILURE)
			{
				free_all_cmds(cmds, 1);
				return (-1);
			}
		}
		cur = cur->next;
	}
	if (cmds && cmds->next)
		exec_pipeline(cmds, envp);
	else
	{
		check_args(cmds, envp);
	}
	free_envp(envp);
	envp = NULL;
	return (1);
}
