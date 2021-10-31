#include "main.h"

void builtins_exit_status(t_elem *elem, char *cmd, char *arg, char *msg)
{
    ft_putstr_fd("minishell: ",2);
    ft_putstr_fd(cmd,2);
    ft_putstr_fd(": ",2);
    if(arg && *arg)
    {
        ft_putstr_fd("`",2);
        ft_putstr_fd(arg,2);
        ft_putstr_fd("': ",2);
    }
    if(!msg || !*msg)
        ft_putendl_fd(strerror(errno), 2);
    else
        ft_putendl_fd(msg, 2);
    elem->data->exit_status = 1;
}