//
// Created by fcody on 10/21/21.
//

#include "main.h"

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
    while(elem->cmd[i])
    {
        ft_putstr_fd(elem->cmd[i], write_fd);
        if(elem->cmd[i + 1])
            write(1, " ", 1);
        i++;
    }
    if(!is_n_flag)
        ft_putstr_fd("\n", write_fd);
}

void    builtin_exit(t_elem *elem, int write_fd)
{
    exit(0);
}