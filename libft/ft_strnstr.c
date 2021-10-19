#include "libft.h"

static int	ft_check_needle(char *haystack_res, const char*needle)
{
	int	needle_size;

	needle_size = ft_strlen(needle);
	while (*needle)
	{
		if (*haystack_res == *needle)
		{
			haystack_res++;
			needle++;
		}
		else
			return (0);
	}
	return (1);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*haystack_res;
	int		haystack_size;

	haystack_res = (char *)haystack;
	haystack_size = ft_strlen(haystack);
	if (!(*needle))
		return ((char *)haystack);
	while (ft_strlen(haystack_res) && len >= ft_strlen(needle))
	{
		if (ft_check_needle(haystack_res, needle) == 1)
			return (haystack_res);
		haystack_res++;
		len--;
	}
	return (0);
}
