/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:36 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:14:54 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	close_fd(t_elem *elem)
{
	if (elem->type == CMD && elem->prev && !elem->next)
		close(elem->prev->pfd[0]);
	else if (elem->type == PIPE && !elem->prev)
		close(elem->pfd[1]);
	else if (elem->type == PIPE && elem->prev && elem->next)
	{
		close(elem->prev->pfd[0]);
		close(elem->pfd[1]);
	}
}

void	closing(t_data *data)
{
	free_arr(data->envp);
}

void	free_arr(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	str = NULL;
}
