/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:05:15 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/01 19:25:39 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_interrupt;

typedef struct s_list
{
	char						*str;
	struct s_list				*prev;
	struct s_list				*next;
}								t_list;

typedef struct s_cmd_exec
{
	char						*raw_cmd;
	char						**args;
	char						*path;
}								t_cmd_exec;

typedef struct s_data
{
	char						*input;
	int							nb_cmd;
	int							lec_save;
}								t_data;

typedef struct s_cmd
{
	t_data						*data;
	int							id;
	char						**args;
	int							nb_args;
	int							*quoted;
	char						*infile;
	int							ascending;
	char						*outfile;
	int							descending;
	bool						pipe_in;
	bool						pipe_out;
	struct s_cmd				*next;
	char						**envp;
	t_list						*env;
	int							last_exit_code;
	pid_t						*pids;
	int							cmd_count;
	void						*command_array;

}								t_cmd;

int								parsing(char *input, t_cmd ***command,
									t_list *env_list, int lec);
void							print_data(t_cmd **command);
int								check_info(t_cmd *cmds, t_cmd **command,
									char **envp);
char							*adjust_path_length(char *start,
									int term_width);
									
void	cleanup_and_exit_child_t_cmd(t_cmd *cmds, char **envp,
				t_cmd **cleaned_args, int exit_code);
void							cleanup_and_exit_child(t_cmd *cmds,
									char **envp, char **cleaned_args,
									int exit_code);
void							sigquit_handler(int signo);
void							free_all_before_exec(t_cmd *current_cmd);
void							sigint_handler(int signo);
size_t							calculate_prompt_length(char *code,
									char *start);
void							update_command_data(t_cmd **cmds,
									t_cmd **command, int save_exit,
									t_list *env_list);
char							*build_prompt(t_cmd *cmds);
void							free_env_list(t_list *env_list);
void							setup_parent_signals(void);
void							setup_child_signals(void);
int								process_input_loop(t_cmd *cmds,
									t_list *env_list);
int								wait_for_child(pid_t pid);
void							ft_putstr_fd(char *s, int fd);
void							free_clean_args(char **new_args, int j);
void							free_cmd(t_cmd_exec *cmd);
void							free_all_cmds(t_cmd *cmds, int is_free);
void							exit_input(t_cmd *cmds, char **env);
void							unset_input(t_cmd *cmds, t_list **env);
int								process_single_cmd(t_cmd *current, t_cmd *cmds,
									char **envp, int *prev_fd);
void							exit_input_pipeline(t_cmd *cmds, char **envp);
pid_t							fork_and_handle_child(t_cmd *current,
									t_cmd *cmds, int prev_fd, int *pipefd);
int								create_pipe_if_needed(t_cmd *current,
									int *pipefd);
void							env_input(t_cmd *cmds, char **envp);
void							free_cleaned_args(char **cleaned_args);
t_list							*create_env_list(char **envp);
void							pwd_input(t_cmd *cmds);
char							**env_list_to_envp(t_list *env_list);
void							free_string_array(char **array);
void							free_cleaned_args(char **cleaned_args);
void							free_clean_args(char **new_args, int j);
int								process_lst(t_cmd **command, int i);
bool							process_a(char *input);
void							free_one_cmd(t_cmd *cmd, int is_free);
void							free_envp(char **envp);
int								process(t_cmd **cmds, t_cmd **command,
									t_list *env_list, char **envp);
void							process_c(t_cmd **command);

#endif