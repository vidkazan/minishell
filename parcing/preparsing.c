/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 19:53:42 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/29 12:58:07 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	main_preparser(t_data *data, char *line)
{
	int	prev_end;
	int	i;

	i = 0;
	prev_end = 0;
	data->elem_start = NULL;
	if (make_redirect_elems(data, line))
		return (1);
	while (line[i])
	{
		quotes(i, data);
		if (line[i] == '|' && !data->q1 && !data->q2)
		{
			if (new_pipe_elem(prev_end, i, data))
			{
				builtins_error(data, NULL, NULL, "syntax error near token '|'");
				data->exit_status = 2;
				data->exec = 0;
				return (1);
			}
			prev_end = i + 1;
		}
		i++;
	}
	return (last_cmd_parser(data, prev_end, i));
}
