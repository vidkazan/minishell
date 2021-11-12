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

void builtin_unset(t_elem *elem)
{
    int index = 0;
    int i;
    int count;
    char *find;
    int flag;

    if(!elem->cmd[1] || !elem->cmd[1][0])
    {
        builtins_error(elem, "unset",NULL, "not enough arguments", 0);
        return;
    }
    count = 0;
    while(elem->cmd[++count])
    {
        flag = 0;
        find = ft_strjoin(elem->cmd[count], "=");
        if (!search_strings_in_array(elem->data->envp, find, &index))
        {
//            builtins_error(elem, "unset", elem->cmd[count], "not a valid identifier", 0);
            flag = 1;
        }
        free(find);
        if(!flag)
        {
            i = index;
            free(elem->data->envp[i]);
            while (elem->data->envp[i + 1])
            {
                elem->data->envp[i] = elem->data->envp[i + 1];
                i++;
            }
            elem->data->envp[i] = 0;
        }
    }

}