/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:22:39 by marvin            #+#    #+#             */
/*   Updated: 2025/09/18 06:28:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd_utils.h"

void	exec_builtin_free_helper(t_cmd **command)
{
	free_all_cmds(*command, 0);
	free(command);
}
