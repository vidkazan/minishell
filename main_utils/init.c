/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:09 by fcody             #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void	init(t_data *data)
{
	data->exec = 1;
	data->elem_start = NULL;
	data->simple_redirect_input_fd = -1;
	data->simple_redirect_output_fd = -1;
	data->double_redirect_output_fd = -1;
	data->double_redirect_input_fd = -1;
	data->q1 = 0;
	data->q2 = 0;
}

int	data_reboot(t_data *data)
{
	list_cleaner(data->elem_start);
	if (data->line)
	{
		free(data->line);
		data->line = 0;
	}
	init(data);
	return (1);
}
