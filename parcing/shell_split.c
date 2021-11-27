#include "../main.h"

void	del_quotes_helper_init(t_del_quotes_helper *data)
{
	data->i = 0;
	data->j = 0;
	data->q1 = 0;
	data->q2 = 0;
	data->flag = 0;
}

void	del_outer_quotes(char *line)
{
	t_del_quotes_helper	data;

	del_quotes_helper_init(&data);
	while (line[data.i])
	{
		if (line[data.i] == '\'' && !data.q2)
		{
			data.q1 = (data.q1 + 1) % 2;
			data.flag = 1;
		}
		if (line[data.i] == '\"' && !data.q1)
		{
			data.q2 = (data.q2 + 1) % 2;
			data.flag = 1;
		}
		line[data.j] = line[data.i];
		if (!data.flag)
			data.j++;
		data.i++;
		data.flag = 0;
	}
	line[data.j] = 0;
}

int	ft_value_of_strings(char c, t_data *data)
{
	int	word_flag;
	int	num;
	int	i;

	num = 0;
	i = -1;
	word_flag = 0;
	while (data->line[++i])
	{
		if (data->line[i] == '\'' || data->line[i] == '\"')
			quotes(i, data);
		if (data->line[i] != c)
		{
			if (!word_flag)
			{
				word_flag = 1;
				num++;
			}
		}
		if (data->line[i] == c && !data->q1 && !data->q2 && word_flag)
			word_flag = 0;
	}
	data->q1 = 0;
	data->q2 = 0;
	return (num);
}

char	**ft_free_arr(char **arr)
{
	if (arr)
	{
		while (*arr)
		{
			free (*arr);
			*arr = NULL;
			(*arr)++;
		}
		free(arr);
		arr = NULL;
	}
	return ((char **) NULL);
}

int	one_word_lenght(t_data *data, char c, char *s, int *i)
{
	int	res;

	res = 0;
	while ((s[*i] != c || data->q1 || data->q2) && s[*i])
	{
		quotes(*i, data);
		(*i)++;
		res++;
	}
	return (res);
}

char	**ft_write_arr(int value_str, t_data *data, char c, char **arr)
{
	int		i;
	int		n_str;
	int		n_sum;
	char	*s;

	s = data->line;
	n_str = 0;
	i = 0;
	while (n_str < value_str)
	{
		n_sum = one_word_lenght(data, c, s, &i);
		if (n_sum)
		{
			arr[n_str] = ft_substr(s, (i - n_sum), n_sum);
			if (arr[n_str] == NULL)
				return (ft_free_arr(arr));
			del_outer_quotes(arr[n_str]);
			n_str++;
		}
		i++;
	}
	arr[n_str] = NULL;
	return (arr);
}

char	**shell_split(char *s, char c)
{
	t_data	data;
	char	**arr;
	int		i;
	int		value_str;

	data.q1 = 0;
	data.q2 = 0;
	data.line = s;
	if (!s)
		return (NULL);
	value_str = ft_value_of_strings(c, &data);
	i = 0;
	arr = (char **)malloc(sizeof(char *) * (value_str + 1));
	if (!arr)
		return (NULL);
	arr = ft_write_arr(value_str, &data, c, arr);
	return (arr);
}
