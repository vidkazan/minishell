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
#include <stdlib.h>



t_elem  *create_elem(t_data *data)
{
    t_elem *ptr;
    ptr = malloc(sizeof(t_elem));
    ptr->type = -1;
    ptr->cmd = NULL;
    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->data = data;
    ptr->pfd[0] = -1;
    ptr->pfd[1] = -1;
    ptr->is_builtin = 0;
    ptr->pid = -1;
    return ptr;
}

t_elem	*delete_current_node(t_elem *elem)
{
	t_elem	*prev_elem;
	t_elem	*next_elem;

	prev_elem = NULL;
	next_elem = NULL;
	if(elem->prev)
		prev_elem = elem->prev;
	if(elem->next)
		next_elem = elem->next;
	elem->cmd = NULL;
	if(!next_elem && !prev_elem)
	{
		elem->data->elem_start = NULL;
		return NULL;
	}
	else if(!next_elem)
	{
		elem->prev = NULL;
		elem = NULL;
		prev_elem->next = NULL;
		return (prev_elem);
	}
	else if(!prev_elem)
	{
		elem->next = NULL;
		elem->data->elem_start = next_elem;
		elem = NULL;
		return (next_elem);
	}
	else
	{
		elem->next = NULL;
		elem->prev = NULL;
		prev_elem->next = next_elem;
		next_elem->prev = prev_elem;
		elem = NULL;
		return prev_elem;
	}
}

t_elem  *push_back(t_elem *ptr, t_data *data)
{
    t_elem *new_elem;
    t_elem *ptr_prev;
    if(ptr == NULL)
        ptr = create_elem(data);
    else
    {
        while(ptr->next != NULL)
        {
            ptr_prev = ptr;
            ptr = ptr->next;
        }
        new_elem = create_elem(data);
        ptr->next = new_elem;
        new_elem->prev = ptr;
    }
    return ptr;
}