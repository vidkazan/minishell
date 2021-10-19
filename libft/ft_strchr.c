#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	symbol;

	symbol = c;
	while (*s)
	{
		if (*s == symbol)
			return ((void *)s);
		s++;
	}
	if (*s == symbol && symbol == 0)
		return ((void *)s);
	return (NULL);
}
