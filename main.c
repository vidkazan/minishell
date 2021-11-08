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

void	read_line_and_add_history(char **line)
{
	if (*line)
	{
		free(*line);
		*line = (char *)NULL;
	}
	*line = readline("minishell $> ");
	if (*line && **line)
		add_history (*line);
}

int main(int ac, char **av, char **env) // not save if no ENVP
{
    t_data *data = malloc(sizeof (t_data));
	int     exit_flag = 0;

    init(data, env);
    data->envp = ft_arrdup(env);
    data->exit_status = 0;
    data->debug = 0;
    data->exec = 1;
    env_path_find(data);
    rl_outstream = stderr;
    if(ac == 2)
        data->debug = 1;
	while (!exit_flag)
	{
		read_line_and_add_history(&data->line);
		// ft_strip(&line);
		if (data->line && !ft_strncmp(data->line, "exit", 5))
		{
            ft_putstr_fd("exit\n", 2);
            exit_flag = 1;
        }
		else if (ft_strlen(data->line) != 0)
		{
			main_preparser(data, data->line);
            while(data->elem_start->prev) // костыыыль
                data->elem_start = data->elem_start->prev;
            redirects(data);
            if(data->exec)
            {
                execution(data->elem_start);
                waiting(data);
            }
            data->exec = 1;
            if(data->debug)
                print_elems(data->elem_start);
			data_reboot(data, NULL, 0);
		}
    }
    closing(data);
    return 0;
}
