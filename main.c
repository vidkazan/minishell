/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:07 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:17:08 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	read_line_and_add_history(char **line)
{
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	*line = readline("minishell $> ");
	if(!*line)
	{
		write(2, "exit\n", 5);
		exit(0);
	}
	if (*line && **line)
		add_history(*line);
}

void	start_init(t_data *data, char **env, int ac, char **av)
{
	ac = 0;
	av = NULL;
	data->std_in = dup(0);
	data->std_out = dup(1);
	init(data);
	data->envp = ft_arrdup(env);
	data->exit_status = 0;
	data->debug = 0;
	data->exec = 1;
	env_path_find(data);
	rl_outstream = stderr;
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = malloc(sizeof (t_data));
	start_init(data, env, ac, av);
	while (1)
	{
		signals_in_main();
		read_line_and_add_history(&data->line);
		vars(data);
		if (data->line && *data->line)
		{
			signals_in_cmd();
			main_preparser(data, data->line);
			redirects(data);
			execution(data->elem_start);
			waiting(data);
		}
		data_reboot(data);
	}
	return (0);
}
