#include "../main.h"

void execution_child_last_cmd(t_elem *elem)
{
	dup2(elem->prev->pfd[0], 0);
	close(elem->prev->pfd[0]);
	close(elem->prev->pfd[1]);
	if (elem->data->double_redirect_output_fd)
		dup2(elem->data->double_redirect_output_fd, 1);
	if (elem->data->simple_redirect_output_fd)
		dup2(elem->data->simple_redirect_output_fd, 1);
}

void execution_child_first_cmd(t_elem *elem)
{
	if (elem->data->double_redirect_output_fd)
		dup2(elem->data->double_redirect_output_fd, 1);
	if (elem->data->simple_redirect_output_fd)
		dup2(elem->data->simple_redirect_output_fd, 1);
	if (elem->data->simple_redirect_input_fd)
		dup2(elem->data->simple_redirect_input_fd, 0);
	if (elem->data->double_redirect_input_fd)
		dup2(elem->data->double_redirect_input_fd, 0);
}

void execution_child_first_pipe(t_elem *elem)
{
	if (elem->data->simple_redirect_input_fd)
		dup2(elem->data->simple_redirect_input_fd, 0);
	if (elem->data->double_redirect_input_fd)
		dup2(elem->data->double_redirect_input_fd, 0);
	dup2(elem->pfd[1], 1);
	close(elem->pfd[0]);
	close(elem->pfd[1]);
}

void execution_child_middle_pipe(t_elem *elem)
{
	dup2(elem->pfd[1], 1);
	dup2(elem->prev->pfd[0], 0);
	close(elem->pfd[0]);
	close(elem->pfd[1]);
	close(elem->prev->pfd[0]);
	close(elem->prev->pfd[1]);
}

void execution_child(t_elem *elem)
{
	if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
		execution_child_last_cmd(elem);
	else if (elem->type == CMD && !elem->prev && !elem->next) // first_CMD
		execution_child_first_cmd(elem);
	else if (elem->type == PIPE && !elem->prev) // first_PIPE
		execution_child_first_pipe(elem);
	else if (elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
		execution_child_middle_pipe(elem);
	if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
	{
		execve_error(elem, elem->cmd[0], NULL, NULL);
		data_reboot(elem->data, NULL, 0);
		exit(elem->data->exit_status);
	}
}