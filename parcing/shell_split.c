/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 20:02:39 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/28 20:02:40 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

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
