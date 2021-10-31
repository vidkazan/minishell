#include "main.h"

//rl_clear_history, rl_on_new_line,
//rl_replace_line, rl_redisplay, add_history, printf,
//malloc, free, write, open, read, close, fork, wait,
//waitpid, wait3, wait4, signal, kill, exit, getcwd,
//chdir, stat, lstat, fstat, unlink, execve, dup,
//dup2, pipe, opendir, readdir, closedir, strerror,
//perror, isatty, ttyname, ttyslot, ioctl, getenv,
//tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum,
//tgetstr, tgoto, tputs

int   builtin_fd_gen(t_elem *elem)
{
    if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
    {
        if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
            return 1;
        if(elem->data->double_redirect_output_fd)
            return elem->data->double_redirect_output_fd;
        if(elem->data->simple_redirect_output_fd)
            return elem->data->simple_redirect_output_fd;
    }
    else if(elem->type == CMD && !elem->prev && !elem->next) // first_CMD
    {
        if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
            return 1;
        if(elem->data->double_redirect_output_fd)
            return elem->data->double_redirect_output_fd;
        if(elem->data->simple_redirect_output_fd)
            return elem->data->simple_redirect_output_fd;
    }
    else if(elem->type == PIPE && !elem->prev && elem->next) // first_PIPE
        return elem->pfd[1];
    else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
        return elem->pfd[1];
    write(2, ">>> builtin_fd_gen: else!\n", 10);
        return 1;
}

void builtin_exec(t_elem *elem)
{
    int write_fd;

    write_fd = builtin_fd_gen(elem);
    if(elem->is_builtin == 1)
        builtin_echo(elem, write_fd);
    if(elem->is_builtin == 2)
        builtin_cd(elem, write_fd);
    if(elem->is_builtin == 3)
        builtin_pwd(elem, write_fd);
    if(elem->is_builtin == 4)
    	builtin_env(elem, write_fd);
    if(elem->is_builtin == 5)
        builtin_exit(elem, write_fd);
    if(elem->is_builtin == 6)
        builtin_unset(elem, write_fd);
    if(elem->is_builtin == 7)
        builtin_export(elem, write_fd);
}

void builtin_check(t_elem *elem)
{
    if(!ft_strncmp(elem->cmd[0], "echo", 4)  && ft_strlen(elem->cmd[0]) == 4)
        elem->is_builtin = 1;
    if(!ft_strncmp(elem->cmd[0], "cd", 2)  && ft_strlen(elem->cmd[0]) == 2)
        elem->is_builtin = 2;
    if(!ft_strncmp(elem->cmd[0], "pwd", 3)  && ft_strlen(elem->cmd[0]) == 3)
        elem->is_builtin = 3;
    if(!ft_strncmp(elem->cmd[0], "env", 3)  && ft_strlen(elem->cmd[0]) == 3)
        elem->is_builtin = 4;
    if(!ft_strncmp(elem->cmd[0], "exit", 4)  && ft_strlen(elem->cmd[0]) == 4)
        elem->is_builtin = 5;
    if(!ft_strncmp(elem->cmd[0], "unset", 5)  && ft_strlen(elem->cmd[0]) == 5)
        elem->is_builtin = 6;
    if(!ft_strncmp(elem->cmd[0], "export", 6) && ft_strlen(elem->cmd[0]) == 6)
        elem->is_builtin = 7;
}
