#include "main.h"


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


int main(int ac, char **av, char **env)
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
//			print_elems(data->elem_start);
		}
		data_reboot(data, NULL, 0);
    }
    return 0;
}
