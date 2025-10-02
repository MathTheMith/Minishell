/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parsing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:10 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 00:38:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_PARSING_H
# define INIT_PARSING_H

# include "../libft/libft.h"
# include "minishell.h"

typedef struct s_redir_ctx
{
	const char	*input;
	char		*result;
	int			i;
	int			j;
	char		type;
	int			count;
}	t_redir_ctx;

typedef struct s_split_context
{
	int	cmd_index;
	int	start;
	int	i;
	int	in_single_quote;
	int	in_double_quote;
}	t_split_context;

typedef struct s_cmd_init_params
{
	t_data	*data;
	int		i;
	char	**cmd_pipe;
	t_list	*env_list;
}	t_cmd_init_params;

/* Command initialization */
void	init_command(t_cmd ***command, t_data *data, t_list *env_list);
void	init_cmd(t_cmd **command, t_cmd_init_params *params);
int		count_cmd(t_data *data);
int		count_args(char *processed);

/* Argument management */
char	**manage_args(t_cmd *command, char *input_str);

/* Redirection handling */
char	*add_spaces_around_redirections(char *input);
int		check_redir_syntax(t_redir_ctx *ctx, char type);

/* Pipe splitting */
char	**split_by_pipes(char *input);
int		count_pipes(char *input);

/* Pipe utilities */
bool	check_pipe_out(char **cmd_pipe, int i);
bool	check_pipe_in(char **cmd_pipe, int i);
void	update_quote_state_pipe(char c, int *in_single, int *in_double);

/* Memory management */
void	free_failed_init(t_cmd **command, int failed_idx, char **cmd_pipe);
void	free_partial_commands(t_cmd **command, int n);
void	free_result_array(char **result, int count);

#endif