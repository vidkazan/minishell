//
// Created by Felipe Cody on 10/20/21.
//

#include "../main.h"

void redirects(t_data *data)
{
	t_elem *elem = data->elem_start;

	while(elem)
	{
		if(elem->type == SIMPLE_REDIRECT_INPUT && data->exec)
            elem = simple_redirect_input(elem);
        else if(elem->type == DOUBLE_REDIRECT_INPUT && data->exec)
            elem = double_redirect_input(elem);
        else if(elem->type == SIMPLE_REDIRECT_OUTPUT && data->exec)
        	elem = simple_redirect_output(elem);
        else if(elem->type == DOUBLE_REDIRECT_OUTPUT && data->exec)
		    elem = double_redirect_output(elem);
		else if(!data->exec && (elem->type == 3 || elem->type == 4 || elem->type == 5 || elem->type == 6))
		{
			if(data->debug)
				dprintf(2,">>> %d exec = 0 DELNODE %p  %s\n", getpid(),elem, elem->cmd[0]);
			elem = delete_current_node(elem);
		}
		else if(elem->next)
			elem = elem->next;
		else
			break ;
	}
}