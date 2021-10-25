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

void builtin_env(t_elem *elem)
{
	int i;

	i = -1;
	while (elem->data->envp[++i])
		dprintf(elem->data->std_out,"%s\n",elem->data->envp[i]);
}

void builtin_unset(t_elem *elem)
{
    int index = 0;
    int i;
    char *find;
    if(!elem->cmd[1] || !elem->cmd[1][0])
    {
        dprintf(2, "unset: not enough arguments\n");
        return;
    }
    find = ft_strjoin(elem->cmd[1], "=");
    if(!search_strings_in_array(elem->data->envp, find, &index))
    {
        dprintf(2, "unset: not found\n");
        free(find);
        return;
    }
    free(find);
    i = index;
    free(elem->data->envp[i]);
    while(elem->data->envp[i + 1])
    {
        elem->data->envp[i] = elem->data->envp[i + 1];
        i++;
    }
    elem->data->envp[i] = 0;
}