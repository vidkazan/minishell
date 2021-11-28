#include "../main.h"

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