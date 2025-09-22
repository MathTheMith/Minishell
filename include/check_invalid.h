/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_invalid.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:00:23 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:00:24 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_INVALID_H
# define CHECK_INVALID_H

# include "minishell.h"

int	print_syntax_error(const char *token);
int	check_invalid_start(char *input, int i);
int	check_invalid_middle(char *input, int i);
int	check_invalid(char *input);

#endif
