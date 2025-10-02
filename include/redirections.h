/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 01:14:55 by marvin            #+#    #+#             */
/*   Updated: 2025/10/02 01:14:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "minishell.h"

typedef struct s_buffer
{
	char	*data;
	size_t	ri;
}	t_buffer;

typedef struct s_expand
{
	const char	*line;
	size_t		*i;
	char		*result;
	size_t		*ri;
	t_list		*env;
}	t_expand;

/* Variable expansion */
char	*expand_variables(const char *line, t_list *env);
char	*expand_result(t_expand *ctx);
char	*get_env_value(t_list *env, const char *name);

/* Redirection validation */
int		bad_redirection(char **args);
void	write_syntax_error(char *token);
int		check_redirect_token(char **args, int i);
int		check_multiple_redirects(char *str);

/* Output redirections */
int		handle_output_redirect(t_cmd *cmd, char *filename);
int		handle_append_redirect(t_cmd *cmd, char *filename);

/* Input redirections */
int		handle_input_redirect(t_cmd *cmd, char *filename);

/* Heredoc functions - exported for use in redirect_main.c */
char	*remove_quotes_from_delimiter(char *delimiter);
int		read_heredoc_loop(char *clean_delim, t_cmd *cmd, char **buffer,
			size_t *bufsize);

/* Heredoc utilities */
int		is_delimiter_quoted(char *delimiter);

/* Main redirection handler */
int		handle_redirections(t_cmd *cmd);

#endif