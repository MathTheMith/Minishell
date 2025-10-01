/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:15:48 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 00:32:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_input_conditions(char *input, t_cmd *cmds)
{
	if ((input[0] == '!' && input[1] == '\0'))
	{
		cmds->last_exit_code = 1;
		return (1);
	}
	if ((input[0] == ':' && input[1] == '\0'))
	{
		cmds->last_exit_code = 0;
		return (1);
	}
	if (input[0] == '\0')
		return (1);
	add_history(input);
	if (input[0] == '~' && input[1] == '\0')
	{
		cmds->last_exit_code = 126;
		ft_putstr_fd("minishell: Is a directory\n", 2);
		return (1);
	}
	return (0);
}

static char	*get_and_check_input(t_cmd *cmds)
{
	char	*prompt;
	char	*input;

	prompt = build_prompt(cmds);
	if (!prompt)
		return (NULL);
	input = readline(prompt);
	free(prompt);
	if (g_interrupt == 130)
	{
		cmds->last_exit_code = 130;
		g_interrupt = 0;
	}
	if (!input)
		return (NULL);
	if (check_input_conditions(input, cmds))
	{
		free(input);
		return (get_and_check_input(cmds));
	}
	return (input);
}

int	process_input_loop(t_cmd *cmds, t_list *env_list)
{
	int		status;
	char	*input;
	t_cmd	**command;
	t_data	*data_ptr;

	status = 0;
	while (1)
	{
		input = get_and_check_input(cmds);
		command = NULL;
		data_ptr = NULL;
		if (process_a(input))
			break ;
		if (parsing(input, &command, env_list, cmds->last_exit_code) == -1)
			handle_parsing_error(command, input, cmds);
		else
		{
			if (command && *command)
				data_ptr = (*command)->data;
			status = execute_command(&cmds, command, env_list, data_ptr);
		}
	}
	if (cmds)
		free_all_cmds(cmds, 1);
	return (status);
}
