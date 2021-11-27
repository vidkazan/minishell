#include "main.h"

//rl_clear_history, rl_on_new_line,
//rl_replace_line, rl_redisplay, add_history, printf,
//malloc, free, write, open, read, close, fork, wait,
//waitpid, wait3, wait4, signal, kill, exit, getcwd,
//chdir, stat, lstat, fstat, unlink, execve, dup,
//dup2, pipe, opendir, readdir, closedir, strerror,
//perror, isatty, ttyname, ttyslot, ioctl, getenv,
//tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum,
//tgetstr, tgoto, tputs

void	init(t_data *data, char **env)
{
	data->exec = 1;
	data->std_in = dup(0);
	data->std_out = dup(1);
	data->elem_start = NULL;
	data->simple_redirect_input_fd = -1;
	data->simple_redirect_output_fd = -1;
	data->double_redirect_output_fd = -1;
	data->double_redirect_input_fd = -1;
	data->q1 = 0;
	data->q2 = 0;
}

int	data_reboot(t_data *data, char *message, int mode)
{
	if (mode)
		printf("%s\n", message);
	list_cleaner(data->elem_start);
	if (data->line)
	{
		free(data->line);
		data->line = 0;
	}
	init(data, data->envp);
	return (1);
}
