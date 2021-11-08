//
// Created by Felipe Cody on 10/20/21.
//

#include "main.h"

t_elem	*simple_redirect_input(t_elem *elem)
{
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
        builtins_error(elem, elem->cmd[0], NULL, "no such file or directory",1);
        elem = delete_current_node(elem);
        if(elem->type == CMD)
        {
            if(elem->data->debug)
                dprintf(2, ">>> %d EXEC = 0\n", getpid());
            elem->data->exec = 0;
        }
	}
	else
	{
		elem->data->simple_redirect_input_fd = fd;
        if(elem->data->debug)
            dprintf(2, ">>> %d SIMPLE_REDIRECT_INPUT si %d di %d so %d do %d\n", getpid(), elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
        elem = delete_current_node(elem);
	}
    return elem;
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
        if(elem->data->debug)
            dprintf(2, ">>> %d SIMPLE_REDIRECT_OUTPUT si %d di %d so %d do %d\n", getpid(), elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
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
        if(elem->data->debug)
            dprintf(2, ">>> %d DOUBLE_REDIRECT_OUTPUT si %d di %d so %d do %d\n", getpid(),elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
		return elem;
	}
}

t_elem *double_redirect_input(t_elem *elem)
{
    int    heredoc_pfd[2];
    char *line = NULL;
    char *line_nl;
    if (elem->data->debug)
        dprintf(2, ">>> heredoc: pipe\n");
    pipe(heredoc_pfd);
    while(ft_strcmp(elem->cmd[0], line))
    {
        line = readline("> ");
        line_nl = ft_strjoin(line, "\n");
        if (elem->data->debug)
            dprintf(2, ">>> heredoc: line_nl: %s\n", line_nl);
        if(strcmp(elem->cmd[0], line))
        {
            write(heredoc_pfd[1], line_nl, ft_strlen(line_nl));
            free(line_nl);
        }
    }
    close(heredoc_pfd[1]);
    if(elem->data->double_redirect_input_fd)
        close(elem->data->double_redirect_input_fd);
    if(elem->data->simple_redirect_input_fd)
    {
        elem->data->simple_redirect_input_fd = -1;
        close(elem->data->simple_redirect_input_fd);
    }
    elem->data->double_redirect_input_fd = heredoc_pfd[0];
    elem = delete_current_node(elem);
    if(elem->data->debug)
        dprintf(2, ">>> %d DOUBLE_REDIRECT_INPUT si %d di %d so %d do %d\n", getpid(), elem->data->simple_redirect_input_fd, elem->data->double_redirect_input_fd, elem->data->simple_redirect_output_fd, elem->data->double_redirect_output_fd);
    return elem;
}

void redirects(t_data *data)
{
	t_elem *elem = data->elem_start;
	while(elem)
	{
	    if(data->debug)
            dprintf(2, ">>> REDIRECTS %p %s\n", elem, *elem->cmd);
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