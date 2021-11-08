//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_echo_main(int i, t_elem *elem, int write_fd)
{
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
}

void builtin_echo(t_elem *elem,int write_fd)
{
    int is_n_flag;
    int i;

    if(elem->data->debug)
        dprintf(2, ">>> %d echo fd %d error code %d\n", getpid(),write_fd, elem->data->exit_status);
    is_n_flag = 0;
    i = 1;
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
    builtin_echo_main(i, elem, write_fd);
    if(!is_n_flag)
        ft_putstr_fd("\n", write_fd);
    elem->data->exit_status = 0;
}
