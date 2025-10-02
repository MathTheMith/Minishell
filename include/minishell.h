/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 00:38:42 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 01:22:12 by marvin           ###   ########.fr       */
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
	char			*str;
	struct s_list	*prev;
	struct s_list	*next;
}	t_list;

typedef struct s_cmd_exec
{
	char	*raw_cmd;
	char	**args;
	char	*path;
}	t_cmd_exec;

typedef struct s_data
{
	char	*input;
	int		nb_cmd;
	int		lec_save;
}	t_data;

typedef struct s_cmd
{
	t_data			*data;
	int				id;
	char			**args;
	int				nb_args;
	int				*quoted;
	char			*infile;
	int				ascending;
	char			*outfile;
	int				descending;
	bool			pipe_in;
	bool			pipe_out;
	struct s_cmd	*next;
	char			**envp;
	t_list			*env;
	int				last_exit_code;
	pid_t			*pids;
	int				cmd_count;
	void			*command_array;
	char			**heredoc_buffer;
	int				heredoc_count;
}	t_cmd;

/* Main execution */
int		execute_command(t_cmd **cmds, t_cmd **command, t_list *env_list,
			t_data *data_ptr);
int		parsing(char *input, t_cmd ***command, t_list *env_list, int lec);
int		check_info(t_cmd *cmds, t_cmd **command, char **envp);
int		process_input_loop(t_cmd *cmds, t_list *env_list);
int		process(t_cmd **cmds, t_cmd **command, t_list *env_list,
			char **envp);

/* Error handling */
int		handle_parsing_error(t_cmd **command, char *input, t_cmd *cmds);

/* Signal handling */
void	sigquit_handler(int signo);
void	sigint_handler(int signo);
void	setup_parent_signals(void);
void	setup_child_signals(void);
int		wait_for_child(pid_t pid);

/* Prompt */
char	*build_prompt(t_cmd *cmds);
size_t	calculate_prompt_length(char *code, char *start);
char	*adjust_path_length(char *start, int term_width);

/* Memory management */
void	free_cmd(t_cmd_exec *cmd);
void	free_all_cmds(t_cmd *cmds, int is_free);
void	free_one_cmd(t_cmd *cmd, int is_free);
void	free_args_array(char **args);
void	free_cleaned_args(char **cleaned_args);
void	free_clean_args(char **new_args, int j);
void	free_string_array(char **array);
void	free_envp(char **envp);
void	free_env_list(t_list *env_list);
void	cleanup_and_exit_child(t_cmd *cmds, char **envp, char **cleaned_args,
			int exit_code);

/* Environment */
t_list	*create_env_list(char **envp);
char	**env_list_to_envp(t_list *env_list);
void	env_input(t_cmd *cmds, char **envp);

/* Builtins */
void	exit_input(t_cmd *cmds, char **env);
void	exit_input_pipeline(t_cmd *cmds, char **envp);
void	unset_input(t_cmd *cmds, t_list **env);
void	pwd_input(t_cmd *cmds);

/* Process utilities */
bool	process_a(char *input);
int		process_lst(t_cmd **command, int i);
void	process_c(t_cmd **command);

/* Command update */
void	update_command_data(t_cmd **cmds, t_cmd **command, int save_exit,
			t_list *env_list);

/* Debug */
void	print_data(t_cmd **command);

#endif