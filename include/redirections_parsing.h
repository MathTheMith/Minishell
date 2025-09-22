/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parsing.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:23 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:24 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_PARSING_H
# define REDIRECTIONS_PARSING_H

# include "../libft/libft.h"
# include "minishell.h"

char	*check_infile(char *processed);
int		check_ascending(char *processed);
char	*check_outfile(char *processed);
int		check_descending(char *processed);

#endif
