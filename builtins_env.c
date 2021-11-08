//
// Created by fcody on 10/21/21.
//

#include "main.h"

void	edit_env_keys(int env_index, char *new_value, t_data *data)
{
	char *env_name;
	int i;

	i = 0;
	if(env_index < 0 || !new_value[0] || !data->envp[env_index])
		return;

	env_name = data->envp[env_index];
	while(data->envp[env_index][i] && data->envp[env_index][i] != '=')
		i++;
	env_name[i + 1] = 0;
	data->envp[env_index] = ft_strjoin(env_name, new_value);
}

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
            builtins_error(elem, "unset", elem->cmd[count], "not a valid identifier", 0);
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

int is_in_export_line(char *line, char **export_arr)
{
    int i = -1;
    while(export_arr[++i])
    {
        if(!ft_strncmp(line,export_arr[i], ft_strlen(line)))
            return 1;
    }
    return 0;
}

int is_min_arr_line(char *line, char **envp,char **export_arr)
{
    int i = -1;
    int j;
    while(envp[++i])
    {
        if(ft_strncmp(line,envp[i], ft_strlen(line)) && !is_in_export_line(envp[i], export_arr))
        {
            j = 0;
            while (line[j] && envp[i][j] && line[j] == envp[i][j])
                j++;
            if (line[j] > envp[i][j])
                return 0;
        }
    }
    return 1;
}

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
//        dprintf(2, ">>> export_arg_check\n");
    if((!arg || !*arg) || (!ft_isalpha(*arg) && *arg != '_'))
        return 0;
    return 1;
}

char **ft_arrjoin_one_line(char **arr1,char *s2)
{
    dprintf(2, ">>> arrjoin_one_line\n");
    char	**res;
    int		size_all;
    int     i;
    int     len1;

    len1 = ft_strlen_arr(arr1);
    if (!arr1 || !s2)
        return (NULL);
    size_all = len1 + 1;
    res = malloc(sizeof(char *) * (size_all + 1));
    if (!res)
        return (NULL);
    res[size_all] = NULL;
    i = 0;
    while(i < len1)
    {
        res[i] = arr1[i];
        i++;
    }
    res[i] = s2;
    free(arr1);
    arr1 = NULL;
    return (res);
}

void export_set(t_elem  *elem)
{
    char *new_line;
    int i;
    int j;
    int index;
    int k;
    char *search_line;

    if(*elem->cmd[1] == '=')
    {
        builtins_error(elem, "export", elem->cmd[1], "not a valid identifier", 1);
        return;
    }
    j = 0;
    while (elem->cmd[++j])
    {
        if(!export_argument_check(elem->cmd[j]))
            builtins_error(elem, "export", elem->cmd[j], "not a valid identifier", 1);
        else
        {
            k = 0;
            while(elem->cmd[j][k] && elem->cmd[j][k] != '=')
                k++;
            if(k == ft_strlen(elem->cmd[j]))
                search_line = ft_strdup(elem->cmd[j]);
            else
                search_line = ft_substr(elem->cmd[j], 0, k);
//            dprintf(2, ">>> search line %s\n", search_line);
            i = -1;
            while (elem->data->envp[++i])
            {
//                if (search_strings_in_array(elem->data->envp, elem->cmd[j], &index))
                if (search_strings_in_array(elem->data->envp, search_line, &index))
                {
//                    dprintf(2, ">>> free_envp %s index %d\n", elem->data->envp[index], index);
                    free(elem->data->envp[index]);
//                    dprintf(2, ">>> envp_index_new\n");
                    elem->data->envp[index] = elem->cmd[j];
                    free(search_line);
                    search_line = NULL;
                    break;
                }
            }
            free(search_line);
            search_line = NULL;
//            dprintf(2, ">>> add_valid_identifier %s\n", elem->cmd[j]);
            new_line = elem->cmd[j];
//            dprintf(2, ">>> add_valid_identifier %s\n", new_line);
            elem->data->envp = ft_arrjoin_one_line(elem->data->envp, new_line);
        }
    }
}

void builtin_export(t_elem *elem, int write_fd) // no handle of replace strings without adding doubles
{
    if(!elem->cmd[1])
        export_sort_print(elem, write_fd);
    else
        export_set(elem);
}