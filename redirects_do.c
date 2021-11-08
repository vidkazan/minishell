//
// Created by Felipe Cody on 10/20/21.
//

#include "main.h"

t_elem	*double_redirect_output(t_elem	*elem)
{
	int    fd;

	fd = open(elem->cmd[0], O_RDWR | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		ft_putstr_fd("error: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
	else
	{
		close(elem->data->simple_redirect_output_fd);
		elem->data->simple_redirect_output_fd = -1;
		elem->data->double_redirect_output_fd = fd;
		elem = delete_current_node(elem);
        if(elem->data->debug)
            dprintf(2, ">>> %d DOUBLE_REDIRECT_OUTPUT si %d di %d so %d do %d\n", getpid(),elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
		return elem;
	}
}
