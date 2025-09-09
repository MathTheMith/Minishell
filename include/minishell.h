
#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
#include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
#include <limits.h>
# include <fcntl.h>

extern volatile sig_atomic_t	g_interrupt;

typedef struct s_list
{
	char			*str;
	struct s_list	*prev;
	struct s_list	*next;
}					t_list;

typedef struct s_cmd_exec
{
	char *raw_cmd;
	char **args;
	char *path;
} t_cmd_exec;

typedef struct s_data
{
	char *input;  //str que l'on recoit
	int nb_cmd;
	int lec_save;
}				t_data;


typedef struct s_cmd
{
	t_data			*data;  //stucture commune
	int				id;       //nb_cmd  
	char 			**args;   // cmd && arg
	int				nb_args;
	int				*quoted;
	char			*infile; // NULL  || prout
	int				ascending;  // <  || <<  || NULL
	char			*outfile; // NULL  || prout
	int				descending; // >  ||  >>  || NULL
	bool			pipe_in; //  |...
	bool			pipe_out; //  ...|
	struct s_cmd	*next; // t_cmd  ||  NULL
	char 			**envp;
	t_list 			*env;
	int				last_exit_code;

}					t_cmd;




//================parsing===================
int parsing(char *input, t_cmd ***command, t_list *env_list, int lec);
void init_command(t_cmd ***command, t_data *data, t_list *env_list);

//================init_struct_utils=============
int count_cmd(t_data *data);
int count_args(char *processed);
void exec_pipeline(t_cmd *cmds, char **envp);
char *check_infile(char *processed);
int check_ascending(char *processed);
char *check_outfile(char *processed);
int check_descending(char *processed);

//===============print_data=================
void print_data(t_cmd **command);

//===============ma
char	**manage_args(t_cmd *command, char *input_str);

//===============manage_quote===============
int	handle_single_quotes(char *str, char *out, int *j, int *k);
int	handle_env_variable(t_cmd *command, char *str, char *out, int *j, int *k);
int	handle_double_quotes(t_cmd *command, char *str, char *out, int *j, int *k);

//===============smart_split===============
void	free_args(char **args, int last_index);
char	**allocate_args(int count);
bool	fill_args(t_cmd *command, char **args, char *str, int arg_count);
char	**smart_split_and_parse(t_cmd *command, char *str);
char **smart_split(t_cmd *cmd, char *input);

//===============smart_split_utils===============
int		count_string_args(const char *str);
void	parse_input_string(t_cmd *cmd, char *src, char *dest);
char	*extract_and_parse_arg(t_cmd *cmd, char *str, int *pos, int arg_quoted);

int check_invalid(char *input);

//===============================

int	check_start(char *input, int i);
char *extract_var_name(char *str);
int	check_info(t_cmd *cmds, char **envp);
t_cmd	*build_test_data(char *input);
char **remove_redirections(char **args);
void	sigquit_handler(int signo);
void	setup_parent_signals(void);
void	setup_child_signals(void);
int		wait_for_child(pid_t pid);
int	check_args(t_cmd *cmds, char **envp);
void init_cmd(t_cmd **command, t_data *data, int i, char **cmd_pipe, t_list *env_list);
void execute_cmd(t_cmd_exec *cmd_exec, char **envp, char **args, t_cmd *cmd);
char	**ft_split(char const *s, char c);
char	*absolute_path(char *cmd);
char	*look_for_path(char *cmd, char **paths);
void	free_paths(char **paths);
void	ft_putstr_fd(char *s, int fd);
void	free_clean_args(char **new_args, int j);
t_cmd_exec	*init_cmd_exec(t_cmd *cmds, char *raw_cmd, char **envp);
char	*find_path(t_cmd *cmds, char *cmd, char **envp);
void	free_cmd(t_cmd_exec *cmd);
void	free_all_cmds(t_cmd *cmds);
void expand_vars(t_cmd *cmds);
int first_arg(t_cmd *cmds, char *arg);
void exit_input(t_cmd *cmds);
void unset_input(t_cmd *cmds, t_list **env);
void cd_input(t_cmd *cmds);
int		process_single_cmd(t_cmd *current, t_cmd *cmds, char **envp,
			int *prev_fd);
void exit_input_pipeline(t_cmd *cmds);
pid_t	fork_and_handle_child(t_cmd *current, t_cmd *cmds, char **envp,
				int prev_fd, int *pipefd);
void exec_builtin(t_cmd *cmds);
int	create_pipe_if_needed(t_cmd *current, int *pipefd);
void echo_input(t_cmd *cmds);
char *add_spaces_around_redirections(char *input);
int export_input(t_cmd *cmds, t_list **env);
void env_input(t_cmd *cmds);
void	free_cleaned_args(char **cleaned_args);
t_list *create_env_list(char **envp);
int 	is_valid_identifier(const char *str);
void    export_error(char *name);
void    add_to_env(t_list **env, char *name, char *value);
void    print_export_list(t_list *env);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_isalnum(int c);
int	ft_isalpha(int c);
void pwd_input(t_cmd *cmds);
char *get_env_value(t_list *env, const char *name);

bool valid_identifier(char *str);
int exist(char *str, t_list *env);
int count_args_without_redirections(char **args);
int handle_redirections(t_cmd *cmd);

#endif