/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_current_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:16:01 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:16:16 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

t_elem	*del_curr_node_single(t_data *data, t_elem *elem)
{
	free(elem);
	data->elem_start = NULL;
	return (NULL);
}

t_elem	*del_curr_node_last(t_elem *elem, t_elem *prev_elem)
{
	elem->prev = NULL;
	free(elem);
	elem = NULL;
	prev_elem->next = NULL;
	return (prev_elem);
}

t_elem	*del_curr_node_first(t_data *data, t_elem *elem, t_elem *next_elem)
{
	elem->next->prev = NULL;
	elem->next = NULL;
	data->elem_start = next_elem;
	free(elem);
	elem = NULL;
	return (next_elem);
}

t_elem	*del_curr_node_mid(t_elem *elem, t_elem *next_elem, t_elem *prev_elem)
{
	elem->next = NULL;
	elem->prev = NULL;
	prev_elem->next = next_elem;
	next_elem->prev = prev_elem;
	free(elem);
	elem = NULL;
	return (prev_elem);
}

t_elem	*delete_current_node(t_data *data, t_elem *elem)
{
	t_elem	*prev_elem;
	t_elem	*next_elem;

	prev_elem = NULL;
	next_elem = NULL;
	if (elem->prev)
		prev_elem = elem->prev;
	if (elem->next)
		next_elem = elem->next;
	if (elem->str)
		free(elem->str);
	free_arr(elem->cmd);
	if (!next_elem && !prev_elem)
		return (del_curr_node_single(data, elem));
	else if (!next_elem)
		return (del_curr_node_last(elem, prev_elem));
	else if (!prev_elem)
		return (del_curr_node_first(data, elem, next_elem));
	else
		return (del_curr_node_mid(elem, next_elem, prev_elem));
}
