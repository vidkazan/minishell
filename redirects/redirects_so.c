/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_so.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:13 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:17:13 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

t_elem	*simple_redirect_output(t_elem *elem)
{
	int	fd;

	fd = open(elem->cmd[0], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		builtins_error(elem->data, elem->cmd[0], NULL, strerror(errno));
		elem->data->exit_status = errno;
		delete_current_node(elem->data, elem);
		return (NULL);
	}
	else
	{
		if (elem->data->double_redirect_output_fd && \
		elem->data->double_redirect_output_fd != \
		elem->data->simple_redirect_output_fd)
			close(elem->data->double_redirect_output_fd);
		elem->data->double_redirect_output_fd = -1;
		elem->data->simple_redirect_output_fd = fd;
		elem = delete_current_node(elem->data, elem);
		return (elem);
	}
}
