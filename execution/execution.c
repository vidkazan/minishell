#include "../main.h"

void execution_not_exec(t_elem *elem)
{
	if (elem->type == PIPE)
	{
		if (pipe(elem->pfd))
		{
			builtins_error(elem->data, "pipe:", NULL, NULL);
			return;
		}
		close_fd(elem);
	}
	elem->data->exec = 1;
}

void execution_child_last_cmd(t_elem *elem)
{
	dup2(elem->prev->pfd[0], 0);
	close(elem->prev->pfd[0]);
	close(elem->prev->pfd[1]);
	if (elem->data->double_redirect_output_fd)
		dup2(elem->data->double_redirect_output_fd, 1);
	if (elem->data->simple_redirect_output_fd)
		dup2(elem->data->simple_redirect_output_fd, 1);
}

void execution_child_first_cmd(t_elem *elem)
{
	if (elem->data->double_redirect_output_fd)
		dup2(elem->data->double_redirect_output_fd, 1);
	if (elem->data->simple_redirect_output_fd)
		dup2(elem->data->simple_redirect_output_fd, 1);
	if (elem->data->simple_redirect_input_fd)
		dup2(elem->data->simple_redirect_input_fd, 0);
	if (elem->data->double_redirect_input_fd)
		dup2(elem->data->double_redirect_input_fd, 0);
}

void execution_child_first_pipe(t_elem *elem)
{
	if (elem->data->simple_redirect_input_fd)
		dup2(elem->data->simple_redirect_input_fd, 0);
	if (elem->data->double_redirect_input_fd)
		dup2(elem->data->double_redirect_input_fd, 0);
	dup2(elem->pfd[1], 1);
	close(elem->pfd[0]);
	close(elem->pfd[1]);
}

void execution_child_middle_pipe(t_elem *elem)
{
	dup2(elem->pfd[1], 1);
	dup2(elem->prev->pfd[0], 0);
	close(elem->pfd[0]);
	close(elem->pfd[1]);
	close(elem->prev->pfd[0]);
	close(elem->prev->pfd[1]);
}

void execution_child(t_elem *elem)
{
	if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
		execution_child_last_cmd(elem);
	else if (elem->type == CMD && !elem->prev && !elem->next) // first_CMD
		execution_child_first_cmd(elem);
	else if (elem->type == PIPE && !elem->prev) // first_PIPE
		execution_child_first_pipe(elem);
	else if (elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
		execution_child_middle_pipe(elem);
	if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
	{
		execve_error(elem, elem->cmd[0], NULL, NULL);
		data_reboot(elem->data, NULL, 0);
		exit(elem->data->exit_status);
	}
}

int execution_fork(t_elem *elem)
{
	elem->pid = fork();
	if (elem->pid < 0)
	{
		builtins_error(elem->data, "fork:", NULL, NULL);
		elem->data->exit_status = 128;
		return 0;
	}
	return 1;
}

void execution_parent_builtin(t_elem *elem)
{
	if (elem->is_builtin)
		builtin_exec(elem);
	close_fd(elem);
}

int	execution_pipe(t_elem *elem)
{
	if ((elem->type == PIPE) && pipe(elem->pfd))
	{
		builtins_error(elem->data, "pipe:", NULL, NULL);
		return 0;
	}
	return 1;
}

void execution(t_elem *elem)
{
    while(elem && elem->cmd)
    {
        if(elem->data->exec)
        {
            builtin_check(elem);
            if (!elem->is_builtin)
                find_path(elem);
            if (!execution_pipe(elem))
				return;
            if (!elem->is_builtin && !execution_fork(elem))
				return;
            if (elem->pid == 0 && !elem->is_builtin)
				execution_child(elem);
            else
				execution_parent_builtin(elem);
        }
        else
			execution_not_exec(elem);
        if (elem->next)
            elem = elem->next;
        else
            break;
    }
}

void waiting(t_data *data)
{
	int status;
	t_elem *elem;

	if(!data->elem_start)
		return;
	elem = data->elem_start;
	status = -1;
	while (elem && data->exec)
	{
	    if(!elem->is_builtin)
	        waitpid(elem->pid, &status, 0);
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