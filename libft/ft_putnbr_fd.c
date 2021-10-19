#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
		if (n > 9)
			ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd(48 + n % 10, fd);
}
