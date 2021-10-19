#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	symbol;
	int				i;

	symbol = c;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == symbol)
			return ((void *)s + i);
		i--;
	}
	return (NULL);
}
