#include "libft.h"

static int	numsize(long n)
{
	int	size;

	size = 0;
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n > 9)
	{
		n /= 10;
		size++;
	}
	if (n < 10)
		size++;
	return (size);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		size;
	long	nl;

	nl = (long)n;
	size = numsize(nl);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str[size] = 0;
	if (nl < 0)
		nl = -nl;
	while (size)
	{
		str[size - 1] = (nl % 10) + 48;
		nl /= 10;
		size--;
	}
	if (str[0] == '0' && n != 0)
		str[0] = '-';
	return (str);
}
