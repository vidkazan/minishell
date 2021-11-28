#include "../main.h"

t_elem	*create_elem(t_data *data)
{
	t_elem	*ptr;

	ptr = malloc(sizeof(t_elem));
	ptr->type = -1;
	ptr->cmd = NULL;
	ptr->next = NULL;
	ptr->prev = NULL;
	ptr->data = data;
	ptr->pfd[0] = -1;
	ptr->pfd[1] = -1;
	ptr->is_builtin = 0;
	ptr->pid = -1;
	return (ptr);
}

t_elem	*push_back(t_elem *start, t_data *data)
{
	t_elem	*ptr;
	t_elem	*new_elem;

	ptr = start;
	if (ptr == NULL)
		new_elem = create_elem(data);
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		new_elem = create_elem(data);
		ptr->next = new_elem;
		new_elem->prev = ptr;
		new_elem->str = NULL;
	}
	return (new_elem);
}

void	list_cleaner(t_elem *elem)
{
	if (!elem)
		return ;
	while (elem->next)
		elem = elem->next;
	while (elem)
		elem = delete_current_node(elem->data, elem);
}
