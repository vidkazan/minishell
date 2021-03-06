/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:00 by fcody             #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

void	builtin_echo_main(int i, t_elem *elem, int write_fd)
{
	while (elem->cmd[i])
	{
		ft_putstr_fd(elem->cmd[i], write_fd);
		if (elem->cmd[i + 1])
			write(write_fd, " ", 1);
		i++;
	}
}

void	builtin_echo(t_elem *elem, int write_fd)
{
	int	is_n_flag;
	int	i;

	is_n_flag = 0;
	i = 1;
	if (!elem->cmd[1])
	{
		ft_putstr_fd("\n", write_fd);
		return ;
	}
	while (elem->cmd[i] && !ft_strncmp(elem->cmd[i], "-n", 2))
	{
		i++;
		is_n_flag++;
	}
	builtin_echo_main(i, elem, write_fd);
	if (!is_n_flag)
		ft_putstr_fd("\n", write_fd);
	elem->data->exit_status = 0;
}
