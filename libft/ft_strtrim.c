#include "libft.h"

static int	ft_set_check(char c, char const *set)
{
	while (*set)
	{
		if (c == *set++)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	pos;
	size_t	end_pos;

	if (!s1 || !set)
		return (NULL);
	pos = 0;
	end_pos = ft_strlen(s1) - 1;
	while (s1[pos] && ft_set_check(s1[pos], set) == 1)
		pos++;
	if (s1[pos] == 0)
		return (ft_calloc(1, sizeof(char)));
	while (end_pos >= 0 && ft_set_check(s1[end_pos], set) == 1)
		end_pos--;
	str = ft_substr(s1, pos, end_pos - pos + 1);
	if (!str)
		return (NULL);
	return (str);
}
