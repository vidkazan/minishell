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
        if (ptr->type == 1)
            printf(">>> CMD:\n");
        if (ptr->type == 2)
            printf(">>> PIPE:\n");
        if (ptr->type == 3)
            printf(">>> S. REDIRECT IN:\n");
        if (ptr->type == 4)
            printf(">>> S. REDIRECT OUT:\n");
        if (ptr->type == 5)
            printf(">>> D. REDIRECT IN:\n");
        if (ptr->type == 6)
            printf(">>> D. REDIRECT OUT:\n");
        /**/printf("^^ ");
        while(*ptr->cmd)
        	/**/printf("%s ", *ptr->cmd++);
        /**/printf("\n\n");
    }
    else
    	/**/printf(">>> nocmd \n");
}

void print_count_elems(t_elem *ptr)
{
    int i = 0;
    printf("1\n");

    while (ptr)
    {
        i++;
        ptr = ptr->prev;
    }
    printf("count of prev elems=%d\n\n", i);
    i = 0;
    while (ptr)
    {
        i++;
        ptr = ptr->next;
    }
    printf("count of elems=%d\n\n", i);
}

void print_elems(t_elem *ptr)
{
    int id = 1;
    /**/printf("\n");
    while(ptr->prev)
        ptr = ptr->prev;
    // print_count_elems(ptr);
    while(ptr)
    {
        print_current_elem(ptr, id++);
        if(ptr->next == NULL)
            break;
        ptr = ptr->next;
    }
}