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
