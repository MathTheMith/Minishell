/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:29 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 00:39:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMART_SPLIT_H
# define SMART_SPLIT_H

# include "../libft/libft.h"
# include "minishell.h"

# define MAX_ENV_NAME 256
# define MAX_EXIT_STR 12

typedef struct s_quote_state
{
	int		in_quotes;
	char	quote;
}	t_quote_state;

typedef struct s_arg_info
{
	int	start;
	int	len;
	int	quote_type;
}	t_arg_info;

typedef struct s_parse_context
{
	int	i;
	int	j;
	int	src_len;
}	t_parse_context;

typedef struct s_quote_params
{
	char	*str;
	char	*out;
	int		*j;
	int		*k;
}	t_quote_params;

/* Quote handling */
int		handle_single_quotes(t_quote_params *params);
int		handle_env_variable(t_cmd *command, t_quote_params *params);
int		handle_double_quotes(t_cmd *command, t_quote_params *params);

/* Splitting and parsing */
void	free_args(char **args, int last_index);
char	**allocate_args(int count);
bool	fill_args(t_cmd *command, char **args, char *str, int arg_count);
char	**smart_split_and_parse(t_cmd *command, char *str);
char	**smart_split(t_cmd *cmd, char *input);

/* String utilities */
int		count_string_args(const char *str);
void	parse_input_string(t_cmd *cmd, char *src, char *dest);
char	*extract_and_parse_arg(t_cmd *cmd, char *str, int *pos, int arg_quoted);
void	skip_spaces(const char *str, int *i);
void	update_quote_state(const char c, int *in_quotes, char *quote);
int		get_arg_length(const char *str, int start, int *quote_type);
char	*extract_raw_arg(const char *str, int start, int len);

/* Helper functions */
void	init_parse_context(t_parse_context *ctx, char *src);
int		should_handle_env(char *src, int i);
void	process_character(t_cmd *cmd, char *src, char *dest,
			t_parse_context *ctx);
void	init_arg_info(t_arg_info *info, const char *str, int pos);
char	*create_final_arg(char *parsed_arg);

/* Environment utilities */
int		extract_env_name(const char *str, int *j, char *env_name);
void	copy_env_value_to_out(char *env_val, char *out, int *k);
void	copy_exit_code_to_out(int exit_code, char *out, int *k);

/* Environment expansion */
int		handle_exit_code(t_cmd *command, t_quote_params *params);
int		handle_env_expansion(t_cmd *command, t_quote_params *params);

#endif