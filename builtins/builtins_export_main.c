/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:26 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:14:56 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	edit_env_keys(int env_index, char *new_value, t_data *data)
{
	char	*env_name;
	int		i;

	i = 0;
	if (env_index < 0 || !new_value[0] || !data->envp[env_index])
		return ;
	while (data->envp[env_index][i] && data->envp[env_index][i] != '=')
		i++;
	env_name = ft_substr(data->envp[env_index], 0, i + 1);
	free(data->envp[env_index]);
	data->envp[env_index] = ft_strjoin(env_name, new_value);
	free(new_value);
	free(env_name);
}

int	is_in_export_line(char *line, char **export_arr)
{
	int	i;

	i = -1;
	while (export_arr[++i])
	{
		if (!ft_strncmp(line, export_arr[i], ft_strlen(line)))
			return (1);
	}
	return (0);
}

int	is_min_arr_line(char *line, char **envp, char **export_arr)
{
	int	i;
	int	j;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(line, envp[i], ft_strlen(line)) && \
		!is_in_export_line(envp[i], export_arr))
		{
			j = 0;
			while (line[j] && envp[i][j] && line[j] == envp[i][j])
				j++;
			if (line[j] > envp[i][j])
				return (0);
		}
	}
	return (1);
}

void	builtin_export(t_elem *elem, int write_fd)
{
	if (!elem->cmd[1])
		export_sort(elem, write_fd);
	else
		export_set(elem);
}
