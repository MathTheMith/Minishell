/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:34 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 00:38:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "../libft/libft.h"
# include "minishell.h"

# define ERR_MALLOC "malloc error\n"

/* List operations */
int		lst_size(t_list *env);
int		len_list(t_list *env);
bool	append(t_list **env, char *value);

/* Array conversion */
char	**lst_to_arr(t_list *env);
void	sort_array(char **arr, int len);

/* Export operations */
int		print_error(char *msg);
bool	export_no_args(t_list *env);
bool	valid_identifier(char *str);
int		exist(char *str, t_list *env);
char	*get_var_value(char *var_name, t_list *env);
bool	is_append_operation(char *str);
char	*extract_var_name(char *str);
char	*extract_var_value(char *str);
bool	export_var(char *str, t_list **env);

/* Exit code expansion */
char	*put_last_exit_code(char *str, t_cmd *cmds);

/* Main export function */
int		export_input(t_cmd *cmds, t_list **env);

#endif