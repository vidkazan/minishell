//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_echo(t_elem *elem)
{
    int is_n_flag = 0;
    int i = 0;
    if(!ft_strncmp(elem->cmd[1],"-n", 2))
    {
        i++;
        is_n_flag++;
    }
    while(elem->cmd[++i])
    {
        write(1, elem->cmd[i], ft_strlen(elem->cmd[i]));
        if(elem->cmd[i + 1])
            write(1, " ", 1);
    }
    if(!is_n_flag)
        write(1, "\n", 1);
}