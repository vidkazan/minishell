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

//void    sigint(int sig)
//{
//    dprintf(2, "\nminishell $>");
//}

/*
 *  malloc:
 *
 *
 *
 * */

void	read_line_and_add_history(char **line)
{
	if (*line)
	{
		free(*line);
		*line = (char *)NULL;
	}
//    signal(SIGINT, sigint);
	*line = readline("minishell $> ");
	if (*line && **line)
		add_history (*line);
}

void start_init(t_data *data, char **env, int ac)
{
	init(data, env);
	data->envp = ft_arrdup(env);
	data->exit_status = 0;
	data->debug = 0;
	data->exec = 1;
	env_path_find(data);
	rl_outstream = stderr;
	if(ac == 2)
		data->debug = 1;
}


int main(int ac, char **av, char **env) // not save if no ENVP
{
    t_data *data = malloc(sizeof (t_data));
	start_init(data,env,ac);
	while (1)
	{
		read_line_and_add_history(&data->line);
		vars(data);
		if(data->line && *data->line)
		{
			main_preparser(data, data->line);
			redirects(data);
			execution(data->elem_start);
			waiting(data);
		}
		data_reboot(data, NULL, 0);
    }
    return 0;
}
