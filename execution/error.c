#include "../main.h"

void builtins_error(t_data *data, char *cmd, char *arg, char *msg)
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
    data->exit_status = 1;
}

void execve_error(t_elem *elem, char *cmd, char *arg, char *msg)
{
    ft_putstr_fd("minishell: ",2);
    ft_putstr_fd(cmd,2);
    ft_putstr_fd(": ",2);

    elem->data->exit_status = error_code_transform();
    if(arg && *arg)
    {
        ft_putstr_fd("`",2);
        ft_putstr_fd(arg,2);
        ft_putstr_fd("': ",2);
    }
    if(elem->data->exit_status == 127)
        ft_putendl_fd("command not found", 2);
    else if(elem->data->exit_status == 126)
        ft_putendl_fd("permission denied", 2);
    else if(!msg || !*msg)
        ft_putendl_fd(strerror(errno), 2);
    else
        ft_putendl_fd(msg, 2);
}