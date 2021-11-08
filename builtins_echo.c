//
// Created by fcody on 10/21/21.
//

#include "main.h"

void exit_code_print(t_elem *elem, int write_fd)
{
    if(elem->data->exit_status > 255)
        elem->data->exit_status = elem->data->exit_status % 255;
    ft_putnbr_fd(elem->data->exit_status, write_fd);
}

void builtin_echo(t_elem *elem,int write_fd)
{
    if(elem->data->debug)
        dprintf(2, ">>> %d echo fd %d error code %d\n", getpid(),write_fd, elem->data->exit_status);
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
    while (elem->cmd[i])
    {
        if(elem->cmd[i] && !ft_strncmp(elem->cmd[i], "$?", 2)  && ft_strlen(elem->cmd[i]) == 2)
            exit_code_print(elem,write_fd);
        else
            ft_putstr_fd(elem->cmd[i], write_fd);
        if (elem->cmd[i + 1])
            write(write_fd, " ", 1);
        i++;
    }
    if(!is_n_flag)
        ft_putstr_fd("\n", write_fd);
    elem->data->exit_status = 0;
}

void    builtin_exit(t_elem *elem, int write_fd)
{
    long long code;
    int i = -1;

    if(elem->cmd[2])
    {
        builtins_error(elem,"exit", NULL, "too many arguments", 0);
        exit(1);
    }
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
                builtins_error(elem,"exit", NULL, "numeric argument required", 0);
                exit(255);
            }
        }
    }
    code = ft_atoi_overflow(elem->cmd[1]);
    if(elem->data->debug)
        dprintf(2,"%llu\n",code);
    if(code == 255)
    {
        builtins_error(elem,"exit", NULL, "numeric argument required", 0);
        exit(255);
    }
    ft_putendl_fd("exit",2);
    exit(code);
}