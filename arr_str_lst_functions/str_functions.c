#include "../main.h"

void	ft_strip(char **str)
{
	int	i;

	if (*str != NULL)
	{
		i = ft_strlen(*str);
		while ((*str)[i - 1] == ' ' && i > 0)
			i--;
		(*str)[i] = '\0';
		while (**str && **str == ' ')
			(*str)++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	if (!s1 && !s2)
		return (0);
	else if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

long long	ft_atoi_overflow(const char *str)
{
	int			neg;
	long long	res;

	res = 0;
	neg = 1;
	while (*str == 32 || *str == '\n' || *str == '\f' \
		|| *str == '\v' || *str == '\r' || *str == '\t')
		str++;
	if (*str == '-')
		neg = -1;
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = (res * 10) + (((int) *str++) - 48);
		if (*str && ((INT64_MAX / 10 < res * neg) \
		|| (INT64_MIN / 10 > res * neg) \
		|| ((INT64_MAX / 10 == res * neg) && ((*str - 48) > INT64_MAX % 10)) || \
		((INT64_MIN / 10 == res * neg) && (-(*str - 48) > INT64_MIN % 10))))
			return (255);
	}
	return (res * neg);
}
