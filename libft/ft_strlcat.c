#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d;
	size_t	s;

	s = 0;
	d = 0;
	while (dst[d] && (d < dstsize))
		d++;
	while (src[s] && (s + d + 1) < dstsize)
	{
		dst[d + s] = src[s];
		s++;
	}
	if (d < dstsize)
		dst[d + s] = 0;
	return (d + ft_strlen(src));
}
