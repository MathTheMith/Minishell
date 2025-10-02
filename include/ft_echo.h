/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:07 by tfournie          #+#    #+#             */
/*   Updated: 2025/10/02 00:38:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ECHO_H
# define FT_ECHO_H

# include "minishell.h"

void	echo_input(t_cmd *cmds);
int		check_big_n(char *str);
int		check_echo_args(t_cmd *cmds);
char	*get_env_value(t_list *env, const char *name);

#endif