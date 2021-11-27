//
// Created by Felipe Cody on 10/20/21.
//

#include "../main.h"

t_elem	*simple_redirect_output(t_elem *elem)
{
	if(elem->data->debug)
		dprintf(2, ">>> %d redirects so %p <%s>\n",getpid(), elem, *elem->cmd);

	int    fd;
	fd = open(elem->cmd[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		builtins_error(elem->data,elem->cmd[0],NULL,strerror(errno));
		elem->data->exit_status = errno;
		elem = delete_current_node(elem->data,elem);
		return NULL;
	}
	else
	{
		if(elem->data->double_redirect_output_fd && elem->data->double_redirect_output_fd != elem->data->simple_redirect_output_fd)
			close(elem->data->double_redirect_output_fd);
		elem->data->double_redirect_output_fd = -1;
		elem->data->simple_redirect_output_fd = fd;
		if(elem->data->debug)
			dprintf(2, ">>> %d SIMPLE_REDIRECT_OUTPUT si %d di %d so %d do %d\n", getpid(), elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
		elem = delete_current_node(elem->data,elem);
        return elem;
    }
}
