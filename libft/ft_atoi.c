#include "libft.h"

int	ft_atoi(const char *str)
{
	int	neg;
	int	res;

	res = 0;
	neg = 1;
	while (*str == 32 || *str == '\n' || *str == '\f' \
		|| *str == '\v' || *str == '\r' || *str == '\t')
		str++;
	if (*str == '-')
		neg = -1;
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
		res = (res * 10) + (((int)*str++) - 48);
	return (res * neg);
}
