/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 19:54:01 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/28 19:54:02 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	del_quotes_helper_init(t_del_quotes_helper *data)
{
	data->i = 0;
	data->j = 0;
	data->q1 = 0;
	data->q2 = 0;
	data->flag = 0;
}

void	del_outer_quotes(char *line)
{
	t_del_quotes_helper	data;

	del_quotes_helper_init(&data);
	while (line[data.i])
	{
		if (line[data.i] == '\'' && !data.q2)
		{
			data.q1 = (data.q1 + 1) % 2;
			data.flag = 1;
		}
		if (line[data.i] == '\"' && !data.q1)
		{
			data.q2 = (data.q2 + 1) % 2;
			data.flag = 1;
		}
		line[data.j] = line[data.i];
		if (!data.flag)
			data.j++;
		data.i++;
		data.flag = 0;
	}
	line[data.j] = 0;
}

void	quotes(int i, t_data *data)
{
	if ((data->line)[i] == '\'' && !data->q1)
		data->q2 = (data->q2 + 1) % 2;
	if ((data->line)[i] == '\"' && !data->q2)
		data->q1 = (data->q1 + 1) % 2;
}
