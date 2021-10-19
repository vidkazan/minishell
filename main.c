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



t_elem  *create_elem()
{
    t_elem *ptr;
    ptr = malloc(sizeof(t_elem));
    ptr->type = 0;
    ptr->cmd = NULL;
    ptr->args = NULL;
    ptr->next = NULL;
    ptr->prev = NULL;
    return ptr;
}

t_elem  *push_back(t_elem *ptr)
{
    t_elem *new_elem;
    t_elem *ptr_prev;

    if(ptr == NULL)
        ptr = create_elem();
    else
    {
        while(ptr->next != NULL)
        {
            ptr_prev = ptr;
            ptr = ptr->next;
        }
        new_elem = create_elem();
        ptr->next = new_elem;
        new_elem->prev = ptr;
    }
    return ptr;
}

void print_current_elem(t_elem *ptr,int id)
{
    printf("| %d | %s |",id, ptr->cmd);
    if(ptr->args && *ptr->args)
    {
        while(*ptr->args)
            printf(" %s | ",*ptr->args++);
        printf("\n");
    }
    else
        printf("| noargs |\n");
}

void print_elems(t_elem *ptr)
{
    int id = 1;
    while(ptr)
    {
        print_current_elem(ptr, id++);
        if(ptr->next == NULL)
            break;
        ptr = ptr->next;
    }
}

void execution(t_data *data)
{
	char *args[2];
	args[0] = "-l"
	pid_t pid1 = fork();
	if(pid1 == 0)
	{
		write(1, "here\n", 5);
		if(execve(data->elem_start->cmd, , data->envp) < 0)
			write(1, "error", 6);
	}
	else
	{
		wait(0);
		write(1, "\nweeeee\n", 8);
	}
}


int main(int ac, char **av, char **env)
{
    int i = 5;
    char *arg[3];
    t_data data;
    t_elem *root_ptr = NULL;

    data.envp = env;
    arg[0] = "-l";
//    arg[1] = "456";
//    arg[1] = 0;
    root_ptr = push_back(root_ptr);
    root_ptr->cmd = "/bin/ls";
    root_ptr->args = arg;
    data.elem_start = root_ptr;
    print_elems(data.elem_start);
    execution(&data);
    return 0;
}