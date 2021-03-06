/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:14:43 by fcody             #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	error_code_transform(void)
{
	if (errno == 2)
		return (127);
	if (errno == 13)
		return (126);
	return (0);
}

void	exit_code_print(t_elem *elem, int write_fd)
{
	if (elem->data->exit_status > 255)
		elem->data->exit_status = elem->data->exit_status % 255;
	ft_putnbr_fd(elem->data->exit_status, write_fd);
}

void	builtin_exit_cases(t_elem *elem)
{
	if (ft_strlen_arr(elem->cmd) > 2)
	{
		builtins_error(elem->data, "exit", NULL, "too many arguments");
		exit(1);
	}
	if (!elem->cmd[1])
	{
		ft_putendl_fd("exit", 2);
		exit(0);
	}
	if (!*elem->cmd[1])
	{
		builtins_error(elem->data, "exit", NULL, "numeric argument required");
		exit(255);
	}
}

void	builtin_exit_check(t_elem *elem, int *i)
{
	while (elem->cmd[1][++(*i)])
	{
		if (!ft_isdigit(elem->cmd[1][*i]))
		{
			builtins_error(elem->data, "exit", NULL, \
			"numeric argument required");
			exit(255);
		}
	}
}

void	builtin_exit(t_elem *elem)
{
	long long	code;
	int			i;

	i = -1;
	builtin_exit_cases(elem);
	if (elem->cmd[1][i + 1] && elem->cmd[1][i + 1] == '-')
		i++;
	builtin_exit_check(elem, &i);
	code = ft_atoi_overflow(elem->cmd[1]);
	if (code == 255)
	{
		builtins_error(elem->data, "exit", NULL, "numeric argument required");
		exit(255);
	}
	ft_putendl_fd("exit", 2);
	exit(code);
}
