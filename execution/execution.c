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

void execution(t_elem *elem)// TODO #100 after handling ""  - fix path finding with spaces etc + check by "echo " | "pwd " FIXME exitcode 256?? export TEST1=LOL TEST2 env | sort | grep -v SHLVL | grep -v _=
{
    while(elem->cmd)
    {
        if(elem->data->exec)
        {
            if (elem->data->debug)
                dprintf(2, ">>> %d NOW %p <%s>\n", getpid(), elem, elem->cmd[0]);
            builtin_check(elem);
            if (elem->data->debug && elem->is_builtin)
                dprintf(2, ">>> %d builtin\n", getpid());
            if (!elem->is_builtin)
                find_path(elem);
            if (elem->type == PIPE)
            {
                if (elem->data->debug)
                    dprintf(2, ">>> %d pipe\n", getpid());
                if (pipe(elem->pfd))
                {
                    builtins_error(elem->data, "pipe:", NULL, NULL, 0);
                    return;
                }
            }
            if (!elem->is_builtin)
            {
                elem->pid = fork();
                if (elem->pid < 0)
                {
                    builtins_error(elem->data, "fork:", NULL, NULL, 128); // FIXME error code?
                    return;
                }
            }
            if (elem->pid == 0 && !elem->is_builtin)
            {
                if (elem->data->debug)
                    dprintf(2, ">>> %d child\n", getpid());
                if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
                {
                    if (elem->data->debug)
                        dprintf(2, ">>> %d last_CMD\n", getpid());
                    dup2(elem->prev->pfd[0], 0);
                    close(elem->prev->pfd[0]);
                    close(elem->prev->pfd[1]);
                    if (elem->data->double_redirect_output_fd)
                        dup2(elem->data->double_redirect_output_fd, 1);
                    if (elem->data->simple_redirect_output_fd)
                        dup2(elem->data->simple_redirect_output_fd, 1);
                }
                else if (elem->type == CMD && !elem->prev && !elem->next) // first_CMD
                {
                    if (elem->data->debug)
                        dprintf(2, ">>> %d first_CMD\n", getpid());
                    if (elem->data->double_redirect_output_fd)
                        dup2(elem->data->double_redirect_output_fd, 1);
                    if (elem->data->simple_redirect_output_fd)
                        dup2(elem->data->simple_redirect_output_fd, 1);
                    if (elem->data->simple_redirect_input_fd)
                        dup2(elem->data->simple_redirect_input_fd, 0);
                    if (elem->data->double_redirect_input_fd)
                        dup2(elem->data->double_redirect_input_fd, 0);
                }
                else if (elem->type == PIPE && !elem->prev) // first_PIPE
                {
                    if (elem->data->debug)
                        dprintf(2, ">>> %d first_PIPE\n", getpid());
                    if (elem->data->simple_redirect_input_fd)
                        dup2(elem->data->simple_redirect_input_fd, 0);
                    if (elem->data->double_redirect_input_fd)
                        dup2(elem->data->double_redirect_input_fd, 0);
                    dup2(elem->pfd[1], 1);
                    close(elem->pfd[0]);
                    close(elem->pfd[1]);
                }
                else if (elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
                {
                    if (elem->data->debug)
                        dprintf(2, ">>> %d middle_PIPE\n", getpid());
                    dup2(elem->pfd[1], 1);
                    dup2(elem->prev->pfd[0], 0);
                    close(elem->pfd[0]);
                    close(elem->pfd[1]);
                    close(elem->prev->pfd[0]);
                    close(elem->prev->pfd[1]);
                }
                if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
                {
                    execve_error(elem, elem->cmd[0], NULL, NULL);
					data_reboot(elem->data, NULL, 0);
                    exit(elem->data->exit_status);
                }
            }
            else
            {
                if (elem->is_builtin)
                    builtin_exec(elem);
                if (elem->data->debug)
                    dprintf(2, ">>> %d parent exit status %d\n", getpid(), elem->data->exit_status);
                close_fd(elem);
            }
        }
        else
        {
			if (elem->type == PIPE)
			{
				if (elem->data->debug)
					dprintf(2, ">>> %d pipe\n", getpid());
				if (pipe(elem->pfd))
				{
					builtins_error(elem->data, "pipe:", NULL, NULL, 0);
					return;
				}
				close_fd(elem);
			}
			if(elem->data->debug)
				dprintf(2, ">>> %d skip %s\n", getpid(), elem->cmd[0]);
            if(elem->data->debug)
                dprintf(2, ">>> %d exec = 1\n", getpid());
            elem->data->exec = 1;
        }
        if (elem->next)
        {
            if(elem->data->debug)
                dprintf(2, ">>> %d next %s\n", getpid(), elem->next->cmd[0]);
            elem = elem->next;
        }
        else
            break;
    }
}

void waiting(t_data *data)
{
	if(!data->elem_start)
		return;
    int status = -1;
	t_elem *elem = data->elem_start;
	while (elem && data->exec)
	{
	    if(!elem->is_builtin)
	    {
	        waitpid(elem->pid, &status, 0);
	        if(data->debug)
                dprintf(2, ">>> %d exit_code %d\n",elem->pid, status);
	    }
		if(elem->next)
			elem = elem->next;
		else
            break;
	}
	if(status != -1)
	{
		if(!data->exit_status)
			data->exit_status = status;
		if(elem->data->exit_status > 255)
			elem->data->exit_status = elem->data->exit_status % 255;
	}
}