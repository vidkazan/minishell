#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*str1;
	char	*str2;

	str1 = (char *)src;
	str2 = (char *)dst;
	if (n < 0 || dst == src)
		return (dst);
	while (n--)
		*str2++ = *str1++;
	return (dst);
}
