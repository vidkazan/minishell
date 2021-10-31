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

void execution(t_elem *elem)
{
    while(elem)
    {
        builtin_check(elem);
        if(elem->data->debug && elem->is_builtin)
            dprintf(2, ">>> %d builtin\n", getpid());
        if(!elem->is_builtin)
            find_path(elem);
        if (elem->type == PIPE)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d pipe\n", getpid());
            if(pipe(elem->pfd))
            {
                write(2, strerror(errno), ft_strlen(strerror(errno)));
                return;
            }
        }
        if(!elem->is_builtin)
        {
            elem->pid = fork();
            if (elem->pid < 0)
            {
                write(2, strerror(errno), ft_strlen(strerror(errno)));
                return;
            }
        }
        if (elem->pid == 0 && !elem->is_builtin)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d child\n", getpid());
            if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
            {
                dup2(elem->prev->pfd[0], 0);
                close(elem->prev->pfd[0]);
                close(elem->prev->pfd[1]);
                if(elem->data->double_redirect_output_fd)
                    dup2(elem->data->double_redirect_output_fd, 1);
                if(elem->data->simple_redirect_output_fd)
                    dup2(elem->data->simple_redirect_output_fd, 1);
            }
            else if(elem->type == CMD && !elem->prev && !elem->next) // first_CMD
            {
                if(elem->data->double_redirect_output_fd)
                    dup2(elem->data->double_redirect_output_fd, 1);
                if(elem->data->simple_redirect_output_fd)
                    dup2(elem->data->simple_redirect_output_fd, 1);
                if(elem->data->simple_redirect_input_fd)
                    dup2(elem->data->simple_redirect_input_fd, 0);
            }
            else if(elem->type == PIPE && !elem->prev) // first_PIPE
            {
                if(elem->data->simple_redirect_input_fd)
                    dup2(elem->data->simple_redirect_input_fd, 0);
                dup2(elem->pfd[1], 1);
                close(elem->pfd[0]);
                close(elem->pfd[1]);
            }
            else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
            {
                dup2(elem->pfd[1], 1);
                dup2(elem->prev->pfd[0], 0);
                close(elem->pfd[0]);
                close(elem->pfd[1]);
                close(elem->prev->pfd[0]);
                close(elem->prev->pfd[1]);
            }
            if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
            {
                ft_putstr_fd("minishell: ",2);
                ft_putstr_fd(elem->cmd[0],2);
                ft_putstr_fd(": ",2);
                ft_putnbr_fd(errno,2);
                ft_putendl_fd(strerror(errno),2);
                exit(errno);
            }
        }
        else
        {
            if(elem->is_builtin)
                builtin_exec(elem);
            if(elem->data->debug)
            dprintf(2, ">>> %d parent\n", getpid());
            close_fd(elem);
        }
        if (elem->next)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d next\n", getpid());
            elem = elem->next;
        }
        else
            break;
    }
}

void waiting(t_data *data)
{
    int status = -1;
	t_elem *elem = data->elem_start;
	while (elem)
	{
	    if(!elem->is_builtin)
	    {
	        waitpid(elem->pid, &status, 0);
	        if(data->debug)
            dprintf(2, ">>> pid exit code %d\n", status);
	    }
		if(elem->next)
			elem = elem->next;
		else
            break;
	}
	if(status != -1)
	    data->exit_status = status;
}