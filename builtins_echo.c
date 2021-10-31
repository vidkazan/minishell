//
// Created by fcody on 10/21/21.
//

#include "main.h"

void exit_code_print(t_elem *elem)
{
    if(elem->data->exit_status > 255)
        elem->data->exit_status = elem->data->exit_status % 255;
    ft_putnbr_fd(elem->data->exit_status, 2);
}

void builtin_echo(t_elem *elem,int write_fd)
{
    int is_n_flag = 0;
    int i = 1;
    if(!elem->cmd[1])
    {
        ft_putstr_fd("\n", write_fd);
		return;
    }
    while (elem->cmd[i] && !ft_strncmp(elem->cmd[i],"-n", 2))
    {
        i++;
        is_n_flag++;
    }
    if(elem->cmd[i] && !ft_strncmp(elem->cmd[i], "$?", 2)  && ft_strlen(elem->cmd[i]) == 2)
    {
        exit_code_print(elem);
        i++;
    }
    while(elem->cmd[i])
    {
        ft_putstr_fd(elem->cmd[i], write_fd);
        if(elem->cmd[i + 1])
            write(1, " ", 1);
        i++;
    }
    if(!is_n_flag)
        ft_putstr_fd("\n", write_fd);
    elem->data->exit_status = 0;
}

void    builtin_exit(t_elem *elem, int write_fd)
{
    int code;
    int i = -1;

    if(!elem->cmd[1] || !*elem->cmd[1])
    {
        ft_putendl_fd("exit",2);
        exit(0);
    }
    else
    {
        if(elem->cmd[1][i + 1] && elem->cmd[1][i + 1] == '-')
            i++;
        while(elem->cmd[1][++i])
        {
            if(!ft_isdigit(elem->cmd[1][i]))
            {
                ft_putstr_fd("minishell: exit:", 2);
                ft_putstr_fd(elem->cmd[1], 2);
                ft_putendl_fd(": numeric argument required", 2);
                exit(0);
            }
        }
    }
    code = ft_atoi(elem->cmd[1]);
    ft_putendl_fd("exit",2);
    exit(code);
}