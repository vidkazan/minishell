#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_s;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	sub_s = (char *)malloc(sizeof(char) * len + 1);
	if (!sub_s)
		return (NULL);
	sub_s[len] = 0;
	ft_strlcpy(sub_s, s + start, len + 1);
	return (sub_s);
}
