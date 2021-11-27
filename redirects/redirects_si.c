//
// Created by Felipe Cody on 10/20/21.
//

#include "../main.h"

t_elem	*simple_redirect_input(t_elem *elem)
{
	if(elem->data->debug)
		dprintf(2, ">>> %d redirects si %p <%s>\n",getpid(), elem, *elem->cmd);
	int    fd;

    if(elem->data->double_redirect_input_fd)
    {
        elem->data->double_redirect_input_fd = -1;
        close(elem->data->double_redirect_input_fd);
    }
    if(elem->data->simple_redirect_input_fd && elem->data->double_redirect_input_fd != elem->data->simple_redirect_input_fd)
        close(elem->data->simple_redirect_input_fd);
	fd = open(elem->cmd[0], O_RDONLY);
	if (fd < 0)
	{
        builtins_error(elem->data, elem->cmd[0], NULL, "no such file or directory");
		if(elem->data->debug)
			dprintf(2, ">>> %d  redirects SI exec = 0\n", getpid());
		elem->data->exec = 0;
        elem = delete_current_node(elem->data,elem);
	}
	else
	{
		elem->data->simple_redirect_input_fd = fd;
        if(elem->data->debug)
            dprintf(2, ">>> %d SIMPLE_REDIRECT_INPUT si %d di %d so %d do %d\n", getpid(), elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
        elem = delete_current_node(elem->data,elem);
	}
    return elem;
}