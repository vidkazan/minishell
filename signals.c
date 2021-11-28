#include "main.h"

void	signal_ctrl_slash(int sig)
{
	ft_putendl_fd("Quit: 3", 2);
	(void)sig;
	signal(SIGINT, SIG_IGN);
}

static void	signal_cmd_ctrl_c(int sig)
{
	write(1, "\n", 1);
	(void)sig;
	signal(SIGINT, SIG_IGN);
}

static void	signal_ctrl_c(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	signals_in_cmd(void)
{
	signal(SIGINT, signal_cmd_ctrl_c);
	signal(SIGQUIT, signal_ctrl_slash);
}

void	signals_in_main(void)
{
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_ctrl_c);
}
