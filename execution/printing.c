#include "../main.h"

//rl_clear_history, rl_on_new_line,
//rl_replace_line, rl_redisplay, add_history, printf,
//malloc, free, write, open, read, close, fork, wait,
//waitpid, wait3, wait4, signal, kill, exit, getcwd,
//chdir, stat, lstat, fstat, unlink, execve, dup,
//dup2, pipe, opendir, readdir, closedir, strerror,
//perror, isatty, ttyname, ttyslot, ioctl, getenv,
//tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum,
//tgetstr, tgoto, tputs

void print_current_elem(t_elem *ptr)
{
	dprintf(2,">>> %p, type %d next %p\n", ptr, ptr->type, ptr->next);
}

void print_elems(t_elem *root_ptr)
{
	dprintf(2,">>> \nprintelems\n");
	t_elem *ptr= root_ptr;
    int id = 1;
    if(!ptr)
    {
    	dprintf(2, ">>> (null)\n");
		return;
    }
    while(ptr->prev)
    	ptr = ptr->prev;
    while(ptr)
    {
        print_current_elem(ptr);
        if(!ptr->next)
            break;
        ptr = ptr->next;
    }
}