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

//void execution(t_elem *elem)
//{
//    pipe(elem->pfd);
//    elem->pid = fork();
//    pid_t pid2 = fork();
//    if(elem->pid == 0)
//    {
//        printf(">>> 1\n");
//        if(elem->type == PIPE)
//            dup2(elem->pfd[1], 1);
//        if(execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
//            ft_putstr_fd(strerror(errno), 2);
//    }
//    if(pid2 == 0)
//    {
//        printf(">>> waiting 2\n");
//        waitpid(pid1, NULL, 0);
//        printf(">>> 2\n");
//        close(elem->pfd[1]);
//        dup2(elem->pfd[0], 0);
//        if(execve(elem->next->cmd[0], elem->next->cmd, elem->data->envp) < 0)
//            printf("%s\n", strerror(errno));
//    }
//    else
//    {
//        wait(0);
//        printf(">>> parent\n");
//    }
//}

void execution(t_elem *elem)
{
    while(elem)
    {
        if (elem->type == CMD && elem->prev && !elem->next)
        {
            dup2(elem->prev->pfd[0], 0);
            dup2(elem->data->std_out, 1);
        }
        if (elem->type == PIPE)
        {
            ft_putstr_fd(">>> pipe\n", 2);
            pipe(elem->pfd);
            if(!elem->prev)
            {
                ft_putstr_fd(">>> first_pipe\n", 2);
                dup2(elem->pfd[1], 1);
            }
            else if(elem->prev && elem->next)
            {
                ft_putstr_fd(">>> middle_pipe\n", 2);
                dup2(elem->pfd[1], 1);
                dup2(elem->prev->pfd[0], 0);
            }
        }
        ft_putstr_fd(">>> fork\n", 2);
        elem->pid = fork();
        if (elem->pid == 0)
        {
            ft_putstr_fd(">>> child\n", 2);
            if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
                ft_putstr_fd(strerror(errno), 2);
        }
        else
            wait(0);
        if(elem->prev)
        {
            ft_putstr_fd(">>> closing prev read pfd\n", 2);
            close(elem->prev->pfd[0]);
        }
        if(elem->pfd[1] != -1)
        {
            ft_putstr_fd(">>> closing write pfd\n", 2);
            close(elem->pfd[1]);
        }
        if (elem->next)
            elem = elem->next;
        else
        {
            ft_putstr_fd(">>> breaking\n", 2);
            if(elem->pfd[1] != -1)
            {
                close(elem->pfd[1]);
                close(elem->pfd[0]);
            }
            break;
        }
        ft_putstr_fd(">>> next_elem\n", 2);
    }
}

void init(t_data *data, char **env)
{
    data->envp = env;
    data->std_in = dup(0);
    data->std_out = dup(1);
    data->error = 0;
}

int main(int ac, char **av, char **env)
{
    int i = 5;
    char *cmd[4];
    char *cmd2[3];
    t_data *data = malloc(sizeof (t_data));

    init(data, env);
    data->elem_start = push_back(data->elem_start, data);
    data->elem_start->cmd = cmd;
    data->elem_start->cmd[0] = "/bin/echo";
    data->elem_start->cmd[1] = "blaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    data->elem_start->cmd[2] = "-l";
    data->elem_start->cmd[3] = 0;
    data->elem_start->type = PIPE;

    data->elem_start = push_back(data->elem_start, data);
    data->elem_start->next->cmd = cmd2;
    data->elem_start->next->cmd[0] = "/usr/bin/wc";
    data->elem_start->next->cmd[1] = 0;
    data->elem_start->next->cmd[2] = 0;
    data->elem_start->next->type = CMD;

    execution(data->elem_start);
//    print_elems(data->elem_start);
    return 0;
}
