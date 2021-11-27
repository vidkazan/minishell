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
    int found;
    char *search_line;

    if(*elem->cmd[1] == '=')
    {
        builtins_error(elem->data, "export", elem->cmd[1], "not a valid identifier");
        return;
    }
    j = 0;
    while (elem->cmd[++j]) // going through export ARGS
    {
    	dprintf(2, ">>> now %s\n",elem->cmd[j]);
    	found= 0;
        if(!export_argument_check(elem->cmd[j]))
            builtins_error(elem->data, "export", elem->cmd[j], "not a valid identifier");
        else // ARG is valid
        {
            i = 0;
            while(elem->cmd[j][i] && elem->cmd[j][i] != '=') // while not =
                i++;
            if(i == ft_strlen(elem->cmd[j]))
                search_line = ft_strdup(elem->cmd[j]); // without =
            else
            	search_line = ft_substr(elem->cmd[j], 0, i); // with =
            i = -1;
            while (elem->data->envp[++i]) // for each ARG going through all env keys
            {
                if (search_strings_in_array(elem->data->envp, search_line, &index,0))
                {
                    free(elem->data->envp[index]);
                    elem->data->envp[index] = ft_strdup(elem->cmd[j]);
                    free(search_line);
                    search_line = NULL;
                    found = 1;
                    break;
                }
            }
            free(search_line);
            search_line = NULL;
            if(!found)
            {
				new_line = ft_strdup(elem->cmd[j]);
				elem->data->envp = ft_arrjoin_one_line(elem->data->envp, new_line);
			}
        }
    }
}
