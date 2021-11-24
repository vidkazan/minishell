//
// Created by fcody on 10/21/21.
//

#include "../main.h"

void export_set(t_elem  *elem)
{
    char *new_line;
    int i;
    int j;
    int index;
    char *search_line;

    if(*elem->cmd[1] == '=')
    {
        builtins_error(elem->data, "export", elem->cmd[1], "not a valid identifier", 1);
        return;
    }
    j = 0;
    while (elem->cmd[++j])
    {
        if(!export_argument_check(elem->cmd[j]))
            builtins_error(elem->data, "export", elem->cmd[j], "not a valid identifier", 1);
        else
        {
            i = 0;
            while(elem->cmd[j][i] && elem->cmd[j][i] != '=')
                i++;
            if(i == ft_strlen(elem->cmd[j]))
                search_line = ft_strdup(elem->cmd[j]);
            else
                search_line = ft_substr(elem->cmd[j], 0, i);
            i = -1;
            while (elem->data->envp[++i])
            {
                if (search_strings_in_array(elem->data->envp, search_line, &index))
                {
                    free(elem->data->envp[index]);
                    elem->data->envp[index] = elem->cmd[j];
                    free(search_line);
                    search_line = NULL;
                    break;
                }
            }
            free(search_line);
            search_line = NULL;
            new_line = elem->cmd[j];
            elem->data->envp = ft_arrjoin_one_line(elem->data->envp, new_line);
        }
    }
}
