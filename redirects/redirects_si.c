/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_si.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:15 by fcody             #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

t_elem	*simple_redirect_input(t_elem *elem)
{
	int	fd;

	if (elem->data->double_redirect_input_fd)
	{
		elem->data->double_redirect_input_fd = -1;
		close(elem->data->double_redirect_input_fd);
	}
	if (elem->data->simple_redirect_input_fd && \
	elem->data->double_redirect_input_fd != \
	elem->data->simple_redirect_input_fd)
		close(elem->data->simple_redirect_input_fd);
	fd = open(elem->cmd[0], O_RDONLY);
	if (fd < 0)
	{
		builtins_error(elem->data, elem->cmd[0], NULL, \
		"no such file or directory");
		elem->data->exec = 0;
		elem = delete_current_node(elem->data, elem);
	}
	else
	{
		elem->data->simple_redirect_input_fd = fd;
		elem = delete_current_node(elem->data, elem);
	}
	return (elem);
}
