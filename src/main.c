/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 15:17:53 by marvin            #+#    #+#             */
/*   Updated: 2025/09/07 15:19:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_interrupt = 0;

char *build_prompt(t_cmd *cmds)
{
    char cwd[1024];
    char *prompt;
    char *suffix = "$ ";
    int count = 0;
    int i;
    int len;
    char *start;
	struct winsize w;
    char *blue = "\001\033[38;5;129m\002";
    char *reset = "\001\033[0m\002";
    char *char_last_exit_code;

    if (!getcwd(cwd, sizeof(cwd)))
        return strdup("$ ");
    
    char_last_exit_code = ft_itoa(cmds->last_exit_code);
    if (g_interrupt == 130)
        char_last_exit_code = ft_itoa(130);
    g_interrupt = 0;
    
    len = strlen(cwd);
    i = len - 1;
    while (i >= 0 && count < 2)
    {
        if (cwd[i] == '/')
            count++;
        i--;
    }
    if (count == 2)
        start = &cwd[i + 2];
    else
        {start = cwd;}
	
    int term_width = 80;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        term_width = w.ws_col;
    }
	size_t max_path_len = term_width - 90;
    if (strlen(start) > max_path_len) {
	
        start = &start[strlen(start) - max_path_len];
	}
    size_t total_len = strlen(blue) + strlen(char_last_exit_code) +
                       strlen("➜  ") + strlen(start) +
                       strlen(suffix) + strlen(reset);

    prompt = malloc(total_len);
    if (!prompt)
        return NULL;

    strcpy(prompt, char_last_exit_code);
    strcat(prompt, " ");
    strcat(prompt, blue);
    strcat(prompt, "➜  ");
    strcat(prompt, start);
    strcat(prompt, suffix);
    strcat(prompt, reset);

    free(char_last_exit_code);
    return prompt;
}

void	sigint_handler(int signo)
{
	(void)signo;
	g_interrupt = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int signo)
{
	(void)signo;
}

void	setup_parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	wait_for_child(pid_t pid)
{
	int	status;
	int	exit_code;

	waitpid(pid, &status, 0);
	
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit_code = 130;
		}
		else if (sig == SIGQUIT)
		{
			write(STDERR_FILENO, "Quit: 3\n", 8);
			exit_code = 131;
		}
		else
			exit_code = 128 + sig;
	}
	else if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = 1;
	
	return (exit_code);
}

int	main(int ac, char **av, char **envp)
{
	char *input;
	char *prompt;
	t_cmd *cmds;
	int save_exit = 0;
	t_list *env_list = create_env_list(envp);
	int status;
	t_cmd **command;

	(void)ac;
	(void)av;
	cmds = malloc(sizeof(t_cmd));
    if (!cmds)
       { return 1;}
	ft_memset(cmds, 0, sizeof(t_cmd));
	status = 0;
	
	setup_parent_signals();
	
	cmds->last_exit_code = 0;
	while (1)
	{
		prompt = build_prompt(cmds);
		if (!prompt)
			break;

		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break;
		}
		if (input[0] == '!' && input[1] == '\0')
		{
			cmds->last_exit_code = 1;
			free(input);
			continue;
		}
		if (input[0] == ':' && input[1] == '\0')
		{
			cmds->last_exit_code = 0;
			free(input);
			continue;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}		
		command = NULL;
		if (*input)
			add_history(input);
		if (input[0] == '~' && input[1] == '\0')
		{
			cmds->last_exit_code = 126;
			ft_putstr_fd("minishell: Is a directory\n", 2);
			free(input);
			continue;
		}
		if (parsing(input, &command, env_list, cmds->last_exit_code) == -1)
		{
			cmds->last_exit_code = 2;
			free(input);
			continue;
		}
		if (!command || !*command)
        	print_data(command);
		
		save_exit = cmds->last_exit_code;
		cmds = *command;
		if (!cmds)
		{
			free(input);
			continue;
		}
		cmds->last_exit_code = save_exit;
        cmds->env = env_list;
		status = check_info(cmds, envp);
		free(input);
	}
	rl_clear_history();
	return (status);
}