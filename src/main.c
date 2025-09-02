/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:56:43 by marvin            #+#    #+#             */
/*   Updated: 2025/08/29 12:30:26 by mvachon          ###   ########lyon.fr   */
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
		// printf("...\n");
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
	// ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
}

void debug_terminal_state()
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        fprintf(stderr, "Terminal size: %d cols x %d rows\n", w.ws_col, w.ws_row);
    }
    
    // Tester si le terminal supporte les couleurs
    char *term = getenv("TERM");
    fprintf(stderr, "TERM: %s\n", term ? term : "undefined");
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
        {return 1;}
	ft_memset(cmds, 0, sizeof(t_cmd));
	status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	// debug_terminal_state();
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
		if (parsing(input, &command, env_list) == -1)
		{
			// ft_putstr_fd("Erreur1 : parsing failed\n", 2);
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
