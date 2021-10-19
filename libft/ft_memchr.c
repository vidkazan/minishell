#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	symbol;

	symbol = c;
	while (n--)
	{
		if (*(unsigned char *)s == symbol)
			return ((void *)s);
		s++;
	}
	return (NULL);
}
