/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:51 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:52 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"

static void	print_export_var(char *str)
{
	int	j;

	j = 0;
	ft_putstr_fd("export ", 1);
	while (str[j] && str[j] != '=')
		ft_putchar_fd(str[j++], 1);
	if (str[j] == '=')
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(&str[j + 1], 1);
		ft_putstr_fd("\"\n", 1);
	}
	else
		ft_putstr_fd("\n", 1);
}

bool	export_no_args(t_list *env)
{
	char	**arr;
	int		i;

	arr = lst_to_arr(env);
	if (!arr)
		return (false);
	sort_array(arr, len_list(env));
	i = 0;
	while (arr[i])
	{
		print_export_var(arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (true);
}
