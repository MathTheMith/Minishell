/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:34 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:00:35 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "../libft/libft.h"
# include "minishell.h"

# define ERR_MALLOC "malloc error\n"

int		lst_size(t_list *env);
int		len_list(t_list *env);
bool	append(t_list **env, char *value);
char	**lst_to_arr(t_list *env);
void	sort_array(char **arr, int len);
int		print_error(char *msg);
bool	export_no_args(t_list *env);
bool	valid_identifier(char *str);
int		exist(char *str, t_list *env);
char	*get_var_value(char *var_name, t_list *env);
bool	is_append_operation(char *str);
char	*extract_var_name(char *str);
char	*extract_var_value(char *str);
bool	export_var(char *str, t_list **env);
char	*put_last_exit_code(char *str, t_cmd *cmds);
int		export_input(t_cmd *cmds, t_list **env);

#endif
