/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 19:54:17 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/29 11:39:03 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	signal_cmd_ctrl_c(int sig)
{
	write(1, "\n", 1);
	(void)sig;
	signal(SIGINT, SIG_IGN);
}

void	signal_ctrl_slash(int sig)
{
	(void)sig;
	ft_putendl_fd("Quit", 2);
	signal(SIGINT, SIG_IGN);
}

void	signals_in_cmd(void)
{
	signal(SIGINT, signal_cmd_ctrl_c);
	signal(SIGQUIT, signal_ctrl_slash);
}

static void	signal_ctrl_c(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	signals_in_main(void)
{
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_ctrl_c);
}
