#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		(*f)(lst->content);
		if (!(lst->next))
			return ;
		lst = lst->next;
	}
}
