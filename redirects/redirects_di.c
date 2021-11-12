//
// Created by Felipe Cody on 10/20/21.
//

#include "../main.h"

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
		if(line && *line)
			free(line);
        line = readline("> ");
        line_nl = ft_strjoin(line, "\n");
        if (elem->data->debug)
            dprintf(2, ">>> heredoc: line_nl: %s\n", line_nl);
        if(strcmp(elem->cmd[0], line))
            write(heredoc_pfd[1], line_nl, ft_strlen(line_nl));
		free(line_nl);
    }
	if(line && *line)
		free(line);
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
