/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:11 by fcody             #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void	redirects(t_data *data)
{
	t_elem	*elem;

	elem = data->elem_start;
	while (elem)
	{
		if (elem->type == SIMPLE_REDIRECT_INPUT && data->exec)
			elem = simple_redirect_input(elem);
		else if (elem->type == DOUBLE_REDIRECT_INPUT && data->exec)
			elem = double_redirect_input(elem);
		else if (elem->type == SIMPLE_REDIRECT_OUTPUT && data->exec)
			elem = simple_redirect_output(elem);
		else if (elem->type == DOUBLE_REDIRECT_OUTPUT && data->exec)
			elem = double_redirect_output(elem);
		else if (!data->exec && (elem->type == 3 || elem->type == 4 \
		|| elem->type == 5 || elem->type == 6))
		{
			elem = delete_current_node(elem->data, elem);
			if (!elem)
				return ;
		}
		else if (elem->next)
			elem = elem->next;
		else
			break ;
	}
}
