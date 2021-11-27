//
// Created by fcody on 10/21/21.
//

#include "../main.h"

void builtin_env(t_elem *elem, int write_fd)
{
	int i;

	i = -1;
	while (elem->data->envp[++i])
	{
	    if(ft_strchr(elem->data->envp[i], '='))
	    {
            ft_putstr_fd(elem->data->envp[i], write_fd);
            ft_putchar_fd('\n', write_fd);
        }
	}
}

void builtin_unset_find(t_elem *elem, int *index, int *flag, int count)
{
	char *find;

	*flag = 0;
	find = ft_strdup(elem->cmd[count]);
	if (!search_strings_in_array(elem->data->envp, find, index, 0))
		*flag = 1;
	free(find);
}

void builtin_unset_rewrite(int index, t_elem *elem)
{
	int i;

	i = index;
	free(elem->data->envp[i]);
	while (elem->data->envp[i + 1])
	{
		elem->data->envp[i] = elem->data->envp[i + 1];
		i++;
	}
	elem->data->envp[i] = 0;
}

void builtin_unset(t_elem *elem)
{
    int index;
    int count;
    int flag;

    index = 0;
    if(!elem->cmd[1] || !elem->cmd[1][0])
    {
        builtins_error(elem->data, "unset",NULL, "not enough arguments");
        return;
    }
    count = 0;
    while(elem->cmd[++count])
    {
		builtin_unset_find(elem, &index, &flag, count);
        if(!flag)
        	builtin_unset_rewrite(index, elem);
    }
}
