#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*str1;
	unsigned char	*str2;

	if (!dst && !src)
		return (0);
	str1 = (unsigned char *)src;
	str2 = (unsigned char *)dst;
	if (src > dst)
	{
		while (len--)
			*str2++ = *str1++;
		len--;
	}
	else
	{
		str1 += len - 1;
		str2 += len - 1;
		while (len--)
			*str2-- = *str1--;
	}
	return (dst);
}
