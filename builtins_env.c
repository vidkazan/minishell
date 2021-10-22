//
// Created by fcody on 10/21/21.
//

#include "main.h"

void	edit_env_keys(int env_index, char *new_value, t_data *data)
{
	char *env_name;
	int i;

//	dprintf(2, "\n>>> new val '%s'\n", new_value);
	i = 0;
	if(env_index < 0 || !new_value[0] || !data->envp[env_index])
		return;

	env_name = data->envp[env_index];
//	dprintf(2, ">>>old %s\n", env_name);
	while(data->envp[env_index][i] && data->envp[env_index][i] != '=')
		i++;
	env_name[i + 1] = 0;
	data->envp[env_index] = ft_strjoin(env_name, new_value);
//	dprintf(2, ">>>new %s\n", data->envp[env_index]);
}

void builtin_env(t_elem *elem)
{
	int i;

	i = -1;
	while (elem->data->envp[++i])
	{
		ft_putstr_fd(elem->data->envp[i], elem->data->std_out);
		write(1, "\n", 1);
	}
}