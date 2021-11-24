//
// Created by Felipe Cody on 10/20/21.
//

#include "../main.h"

void redirects(t_data *data)
{
	t_elem *elem = data->elem_start;

	while(elem && data->exec)
	{
	    if(data->debug)
            dprintf(2, ">>> %d REDIRECTS %p <%s>\n",getpid(), elem, *elem->cmd);
		if(elem->type == SIMPLE_REDIRECT_INPUT)
            elem = simple_redirect_input(elem);
        else if(elem->type == DOUBLE_REDIRECT_INPUT)
            elem = double_redirect_input(elem);
        else if(elem->type == SIMPLE_REDIRECT_OUTPUT)
		    elem = simple_redirect_output(elem);
        else if(elem->type == DOUBLE_REDIRECT_OUTPUT)
		    elem = double_redirect_output(elem);
		else if(elem->next)
			elem = elem->next;
		else
			break ;
	}
}