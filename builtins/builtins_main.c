#include "../main.h"

void builtin_debug(t_elem *elem)
{
    if ( (!elem->cmd[1] && !elem->data->debug )|| (elem->cmd[1] && *elem->cmd[1] == '1'))
    {
        dprintf(2, ">>> debug mode ON\n");
        elem->data->debug++;
        return;
    }
    if( (!elem->cmd[1] && elem->data->debug ) || (elem->cmd[1] && *elem->cmd[1] == '0'))
    {
        dprintf(2, ">>> debug mode OFF\n");
        elem->data->debug = 0;
        return;
    }
}

int   builtin_fd_gen(t_elem *elem)
{
    if ((elem->type == CMD && elem->prev && !elem->next) || (elem->type == CMD && !elem->prev && !elem->next)) // first_last_CMD
    {
        if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
            return 1;
        if(elem->data->double_redirect_output_fd > 2)
            return elem->data->double_redirect_output_fd;
        if(elem->data->simple_redirect_output_fd > 2)
            return elem->data->simple_redirect_output_fd;
    }
    else if(elem->type == PIPE && !elem->prev && elem->next) // first_PIPE
        return elem->pfd[1];
    else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
        return elem->pfd[1];
	return 1;
}

void builtin_exec(t_elem *elem)
{
    int write_fd;

    write_fd = builtin_fd_gen(elem);
    if(elem->is_builtin == 1)
        builtin_echo(elem, write_fd);
    if(elem->is_builtin == 2)
        builtin_cd(elem);
    if(elem->is_builtin == 3)
        builtin_pwd(elem, write_fd);
    if(elem->is_builtin == 4)
    	builtin_env(elem, write_fd);
    if(elem->is_builtin == 5)
        builtin_exit(elem);
    if(elem->is_builtin == 6)
        builtin_unset(elem);
    if(elem->is_builtin == 7)
        builtin_export(elem, write_fd);
//    if(elem->is_builtin == 8)
//        builtin_debug(elem);
}

void builtin_check(t_elem *elem)
{
    if(!ft_strcmp(elem->cmd[0], "echo"))
        elem->is_builtin = 1;
    if(!ft_strcmp(elem->cmd[0], "cd"))
        elem->is_builtin = 2;
    if(!ft_strcmp(elem->cmd[0], "pwd"))
        elem->is_builtin = 3;
    if(!ft_strcmp(elem->cmd[0], "env"))
        elem->is_builtin = 4;
    if(!ft_strcmp(elem->cmd[0], "exit"))
        elem->is_builtin = 5;
    if(!ft_strcmp(elem->cmd[0], "unset"))
        elem->is_builtin = 6;
    if(!ft_strcmp(elem->cmd[0], "export"))
        elem->is_builtin = 7;
//    if(!ft_strcmp(elem->cmd[0], "d"))
//        elem->is_builtin = 8;
}
