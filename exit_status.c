#include "main.h"

void builtins_error(t_elem *elem, char *cmd, char *arg, char *msg, int code)
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

int error_code_transform()
{
    if(errno == 2) // command not found
        return 127;
    if(errno == 13) // permission denied
        return 126;
    return 0;
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

void exit_code_print(t_elem *elem, int write_fd)
{
    if(elem->data->exit_status > 255)
        elem->data->exit_status = elem->data->exit_status % 255;
    ft_putnbr_fd(elem->data->exit_status, write_fd);
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