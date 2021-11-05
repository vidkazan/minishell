#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*res_work;

	if (!lst)
		return (NULL);
	res = ft_lstnew((*f)(lst->content));
	if (!res)
		return (NULL);
	res_work = res;
	while (lst->next)
	{
		ft_lstadd_back(&res_work, ft_lstnew((*f)(lst->content)));
		if (!(res_work->next))
		{
			ft_lstclear(&res, del);
			return (NULL);
		}
		res_work = res_work->next;
		lst = lst->next;
	}
	return (res);
}
