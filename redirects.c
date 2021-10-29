//
// Created by Felipe Cody on 10/20/21.
//

#include "main.h"

t_elem	*simple_redirect_input(t_elem *elem)
{
	int    fd;

	fd = open(elem->cmd[0], O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("error: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
	else
	{
		if(elem->data->simple_redirect_output_fd && elem->data->double_redirect_output_fd != elem->data->simple_redirect_output_fd)
			close(elem->data->simple_redirect_input_fd);
		elem->data->simple_redirect_input_fd = fd;
		elem = delete_current_node(elem);
		return elem;
	}
}

t_elem	*simple_redirect_output(t_elem *elem)
{
	int    fd;

	fd = open(elem->cmd[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		ft_putstr_fd("error: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		exit(1);
	}
	else
	{
		if(elem->data->double_redirect_output_fd && elem->data->double_redirect_output_fd != elem->data->simple_redirect_output_fd)
			close(elem->data->double_redirect_output_fd);
		elem->data->double_redirect_output_fd = -1;
		elem->data->simple_redirect_output_fd = fd;
		elem = delete_current_node(elem);
		return elem;
	}
}

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
		return elem;
	}
}

void simple_redirects(t_data *data)
{
	t_elem *elem = data->elem_start;
	while(elem)
	{
		if(elem->type == SIMPLE_REDIRECT_INPUT)
			elem = simple_redirect_input(elem);
		if(elem->type == SIMPLE_REDIRECT_OUTPUT)
			elem = simple_redirect_output(elem);
		if(elem->type == DOUBLE_REDIRECT_OUTPUT)
			elem = double_redirect_output(elem);
		if(elem->next)
			elem = elem->next;
		else
			break ;
	}
}