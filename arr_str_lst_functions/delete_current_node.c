#include "../main.h"

t_elem	*delete_current_node_single(t_data *data,t_elem *elem)
{
	free(elem);
	data->elem_start = NULL;
	return NULL;
}

t_elem	*delete_current_node_last(t_elem *elem,t_elem	*prev_elem)
{
	elem->prev = NULL;
	free(elem);
	elem = NULL;
	prev_elem->next = NULL;
	return (prev_elem);
}

t_elem	*delete_current_node_first(t_data *data,t_elem *elem,t_elem	*next_elem)
{
	elem->next->prev = NULL;
	elem->next = NULL;
	data->elem_start = next_elem;
	free(elem);
	elem = NULL;
	return (next_elem);
}

t_elem	*delete_current_node_middle(t_elem *elem,t_elem	*next_elem,t_elem	*prev_elem)
{
	elem->next = NULL;
	elem->prev = NULL;
	prev_elem->next = next_elem;
	next_elem->prev = prev_elem;
	free(elem);
	elem = NULL;
	return prev_elem;
}

t_elem	*delete_current_node(t_data *data,t_elem *elem)
{
	t_elem	*prev_elem;
	t_elem	*next_elem;

	prev_elem = NULL;
	next_elem = NULL;
	if(elem->prev)
		prev_elem = elem->prev;
	if(elem->next)
		next_elem = elem->next;
	if(elem->str)
		free(elem->str);
	free_arr(elem->cmd);
	if(!next_elem && !prev_elem)
		return (delete_current_node_single(data, elem));
	else if(!next_elem)
		return (delete_current_node_last(elem, prev_elem));
	else if(!prev_elem)
		return (delete_current_node_first(data,elem,next_elem));
	else
		return (delete_current_node_middle(elem,next_elem,prev_elem));
}