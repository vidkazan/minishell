//
// Created by fcody on 10/21/21.
//

#include "main.h"

void export_sort_print(t_elem *elem, int write_fd)
{
    int i;
    int count = -1;
    int env_arr_len = ft_strlen_arr(elem->data->envp);
    char **export_arr;

    export_arr = ft_calloc(sizeof (char *), env_arr_len + 1);
    while(++count < env_arr_len) // while all lines are not full
    {
        i = -1;
        while(elem->data->envp[++i]) // checking all lines to compare
        {
            if(!is_in_export_line(elem->data->envp[i], export_arr) && is_min_arr_line(elem->data->envp[i], elem->data->envp, export_arr))
            {
                export_arr[count] = elem->data->envp[i];
                break;
            }
        }
    }
    i = -1;
    while (export_arr[++i])
    {
        count = -1;
        ft_putstr_fd("declare -x ",write_fd);
        while(export_arr[i][++count] && export_arr[i][count] != '=')
            ft_putchar_fd(export_arr[i][count], write_fd);
        if(export_arr[i][count] == '=')
        {
            ft_putstr_fd("=\"", write_fd);
            while(export_arr[i][++count])
                ft_putchar_fd(export_arr[i][count], write_fd);
            ft_putstr_fd("\"", write_fd);

        }
        ft_putchar_fd('\n', write_fd);
    }
    free(export_arr);
    export_arr = NULL;
}

int export_argument_check(char *arg)
{
    char *ptr;

    ptr = arg;
//        dprintf(2, ">>> export_arg_check\n");
    if((!ptr || !*ptr) || (!ft_isalpha(*ptr) && *ptr != '_'))
        return 0;
    while(*ptr)
    {
        if(!ft_isalpha(*ptr) && *ptr != '_' && *ptr != '=' && *ptr != '\'' && *ptr != '\"')
            return 0;
        ptr++;
    }
    return 1;
}