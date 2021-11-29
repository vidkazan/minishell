/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 19:53:51 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/29 13:00:33 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	new_pipe_elem(int start, int end, t_data *data)
{
	t_elem	*new;

	new = push_back(data->elem_start, data);
	data->elem_start = new;
	while (new->next)
		new = new->next;
	new->type = 1;
	new->str = malloc(sizeof(char *) + (end - start + 1));
	if (data->line[end] == '|')
		new->type = 2;
	ft_strlcpy(new->str, data->line + start, end - start + 1);
	(new->str)[end - start + 1] = 0;
	new->cmd = shell_split(new->str, ' ');
	if (data->debug)
		printf("%d %d | cmd=^^%s^^, type = %d\n", \
			start, end, new->str, new->type);
	if (!*new->cmd)
		return (1);
	return (0);
}

int	check_last_cmd(t_data *data)
{
	t_elem	*prelast;
	t_elem	*last;

	last = data->elem_start;
	while (last->next)
		last = last->next;
	prelast = last->prev;
	if (prelast && prelast->type > 2)
	{
		delete_current_node(data, last);
		return (0);
	}
	return (1);
}

int	last_cmd_parser(t_data *data, int prev_end, int i)
{
	if (new_pipe_elem(prev_end, i, data))
	{
		while (data->elem_start->prev)
			data->elem_start = data->elem_start->prev;
		if (check_last_cmd(data))
		{
			builtins_error(data, NULL, NULL, \
				"syntax error no cmd");
			data->exit_status = 2;
			data->exec = 0;
			return (1);
		}
	}
	while (data->elem_start->prev)
		data->elem_start = data->elem_start->prev;
	if (data->q1 || data->q2)
	{
		builtins_error(data, NULL, NULL, "Error: Unclosed quotes");
		data->exit_status = 2;
		data->exec = 0;
		return (1);
	}
	return (0);
}
