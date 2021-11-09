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

void print_current_elem(t_elem *ptr,int id)
{
    if(ptr->cmd && *ptr->cmd)
    {
        dprintf(2,">>> %p, type %d next %p", ptr, ptr->type, ptr->next);
        while(*ptr->cmd)
        	dprintf(2," %s ", *ptr->cmd++);
        dprintf(2,"\n");
    }
    else
    	dprintf(2,">>> printelems: nocmd \n");
}

void print_elems(t_elem *ptr)
{
    int id = 1;
    dprintf(2,"\n");
    while(ptr)
    {
        print_current_elem(ptr, id++);
        if(ptr->next == NULL)
            break;
        ptr = ptr->next;
    }
}