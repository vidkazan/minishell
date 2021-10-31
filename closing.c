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

void close_fd(t_elem *elem)
{
    if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
        close(elem->prev->pfd[0]);
    else if(elem->type == PIPE && !elem->prev) // first_PIPE
        close(elem->pfd[1]);
    else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
    {
        close(elem->prev->pfd[0]);
        close(elem->pfd[1]);
    }
}

void closing(t_data *data)
{
    free_arr(data->envp);
}