/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finding.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:45 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:14:50 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

int	access_granted(t_elem *elem, char *filename_with_path)
{
	if (access(filename_with_path, F_OK) > -1)
	{
		free(elem->cmd[0]);
		elem->cmd[0] = filename_with_path;
		return (1);
	}
	return (0);
}

void	path_check(t_elem *elem, char **path_arr, char *file_no_path)
{
	char	*filename_with_path;
	char	*filename_with_slash;
	int		i;

	i = -1;
	while (path_arr[++i])
	{
		filename_with_slash = ft_strjoin(path_arr[i], "/");
		if (!filename_with_slash)
			return ;
		filename_with_path = ft_strjoin(filename_with_slash, \
								file_no_path);
		free(filename_with_slash);
		filename_with_slash = NULL;
		if (!filename_with_path)
			return ;
		if (access_granted(elem, filename_with_path) == 1)
			break ;
		free(filename_with_path);
		filename_with_path = NULL;
	}
}

void	env_path_find(t_data *data)
{
	int		i;

	i = -1;
	while (data->envp[++i])
	{
		if (!ft_strncmp(data->envp[i], "PATH=", 5))
			data->path = (data->envp[i] + 5);
	}
}

int	filename_check(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != '/' && str[i] != '.')
			return (1);
	}
	return (0);
}

void	find_path(t_elem *elem)
{
	char	*filename_no_path;
	char	**path_arr;

	if (!elem)
		return ;
	path_arr = ft_split(elem->data->path, ':');
	if (*elem->cmd[0] && filename_check(elem->cmd[0]))
	{
		filename_no_path = elem->cmd[0];
		path_check(elem, path_arr, filename_no_path);
	}
	free_arr(path_arr);
}
