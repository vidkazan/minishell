/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_set.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:28 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:14:55 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	export_set_add(t_elem *elem, int found, int j)
{
	char	*new_line;

	if (!found)
	{
		new_line = ft_strdup(elem->cmd[j]);
		elem->data->envp = ft_arrjoin_one_line(elem->data->envp, new_line);
	}
}

void	export_set_find(t_elem *elem, int *found, int j, char *search_line)
{
	int	i;
	int	index;

	i = -1;
	while (elem->data->envp[++i])
	{
		if (search_strings_in_array(elem->data->envp, search_line, &index, 0))
		{
			free(elem->data->envp[index]);
			elem->data->envp[index] = ft_strdup(elem->cmd[j]);
			*found = 1;
			break ;
		}
	}
}

char	*export_set_search_line_gen(t_elem *elem, int j)
{
	int	i;

	i = 0;
	while (elem->cmd[j][i] && elem->cmd[j][i] != '=')
		i++;
	if (i == (int)ft_strlen(elem->cmd[j]))
		return (ft_strdup(elem->cmd[j]));
	else
		return (ft_substr(elem->cmd[j], 0, i));
}

void	export_set(t_elem *elem)
{
	int		j;
	int		found;
	char	*search_line;

	j = 0;
	while (elem->cmd[++j])
	{
		found = 0;
		if (!export_argument_check(elem->cmd[j]))
			builtins_error(elem->data, "export", elem->cmd[j], \
			"not a valid identifier");
		else
		{
			search_line = export_set_search_line_gen(elem, j);
			export_set_find(elem, &found, j, search_line);
			free(search_line);
			search_line = NULL;
			export_set_add(elem, found, j);
		}
	}
}
