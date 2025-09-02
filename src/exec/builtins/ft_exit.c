/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:24 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <limits.h>

int ft_atoll_safe(const char *str, long long *out)
{
    unsigned long long result = 0;
    int sign = 1;
    int i = 0;

    *out = 0;
    
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;

    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    if (!(str[i] >= '0' && str[i] <= '9'))
        return 0;

    while (str[i] >= '0' && str[i] <= '9')
    {
        int digit = str[i] - '0';
        if (sign == 1)
        {
            if (result > (unsigned long long)(LLONG_MAX - digit) / 10)
                return 0;
        }
        else
        {
            if (result > ((unsigned long long)LLONG_MAX + 1 - digit) / 10)
                return 0;
        }
        result = result * 10 + digit;
        i++;
    }
    if (sign == -1)
        *out = -(long long)result;
    else
        *out = (long long)result;

    return 1;
}


void exit_input(t_cmd *cmds)
{
    long long num;

    if (!cmds || !cmds->args || !cmds->args[0])
        return;

    if (isatty(STDIN_FILENO))
        ft_putstr_fd("exit\n", 1);

    if (cmds->args[1])
    {
        if (!ft_atoll_safe(cmds->args[1], &num))
        {
            ft_putstr_fd("minishell: exit: ", 2);
            ft_putstr_fd(cmds->args[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            exit(2);
        }
        if (cmds->args[2])
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            cmds->last_exit_code = 1;
            return;
        }
        exit((unsigned char)(num % 256));
    }
    exit(cmds->last_exit_code);
}

void exit_input_pipeline(t_cmd *cmds)
{
    long long num;

    if (!cmds->args[1])
        exit(0);

    if (!ft_atoll_safe(cmds->args[1], &num))
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(cmds->args[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(2);
    }
    if (cmds->args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        exit(1);
    }
    exit((unsigned char)(num % 256));
}
