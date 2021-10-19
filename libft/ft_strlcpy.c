#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	size_src;

	if (!dst || !src)
		return (0);
	size_src = ft_strlen(src);
	if (dstsize == 0)
		return (size_src);
	while (--dstsize && *src)
		*dst++ = *src++;
	*dst = 0;
	return (size_src);
}
