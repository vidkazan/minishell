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


int main(int ac, char **av, char **env)
{
    t_data *data = malloc(sizeof (t_data));
	int		exit_flag = 0;

    init(data);
    data->envp = ft_arrdup(env);
    data->exit_status = 0;
    env_path_find(data);
    if(ac == 2)
        data->debug = 1;
	while (!exit_flag)
	{
		read_line_and_add_history(&data->line);
		// ft_strip(&line);
		if (data->line && !ft_strncmp(data->line, "exit", 5))
			exit_flag = 1;
		else
		{
			main_preparser(data, data->line);
            while(data->elem_start->prev)
            data->elem_start = data->elem_start->prev;
            // simple_redirects(data);
            // execution(data->elem_start);
            // waiting(data);
            // if(data->debug)
                print_elems(data->elem_start);
			data_reboot(data, NULL, 0);
		}
    }
    closing(data);
    return 0;
}

// int main(int argc, char **argv, char **env)
// {
//     t_data data;
//     int exit_flag = 0;
//     // char *line;

//     init(&data);
//     data.envp = ft_arrdup(env);
//     data.exit_status = 0;
//     env_path_find(&data);
//     while(!exit_flag)
//     {
//         data.line = calloc(sizeof(char), 100);
//         scanf("%s", data.line);
//         // printf("%s\n", line);
//         main_preparser(&data, data.line);
//         print_elems(data.elem_start);
//         free(data.line);
//     }
// }