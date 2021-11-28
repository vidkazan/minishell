/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:15:56 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:16:17 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

int	ft_strlen_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr || !arr[0])
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**ft_arrdup(char **arr)
{
	char	**arr_dup;
	int		i;
	int		arr_len;

	if (!arr || !arr[0])
		return (NULL);
	i = -1;
	arr_len = ft_strlen_arr(arr);
	arr_dup = (char **)malloc(sizeof(char *) * (arr_len + 1));
	while (++i < arr_len)
		arr_dup[i] = ft_strdup(arr[i]);
	arr_dup[arr_len] = NULL;
	return (arr_dup);
}

char	**ft_arrjoin(char **s1, char **s2)
{
	char	**res;
	int		size_all;
	int		i;
	int		len1;

	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strlen_arr(s1);
	size_all = len1 + ft_strlen_arr(s2);
	res = malloc(sizeof(char *) * (size_all + 1));
	res[size_all] = NULL;
	i = 0;
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	while (i < size_all)
	{
		res[i] = s2[i - len1];
		i++;
	}
	return (res);
}

char	*search_strings_in_array(char **arr, char *search_word, \
										int *index, int mode)
{
	int	i;

	i = -1;
	if (!search_word)
		return (NULL);
	while (arr[++i])
	{
		if (!ft_strncmp(arr[i], search_word, ft_strlen(search_word)) \
		&& ((!arr[i][ft_strlen(search_word)]) || \
		arr[i][ft_strlen(search_word)] == '='))
		{
			if (index)
				*index = i;
			if (!mode)
				return (arr[i] + ft_strlen(search_word));
			else
				return (arr[i] + ft_strlen(search_word) + 1);
		}
	}
	return (NULL);
}

char	**ft_arrjoin_one_line(char **arr1, char *s2)
{
	char	**res;
	int		size_all;
	int		i;
	int		len1;

	len1 = ft_strlen_arr(arr1);
	if (!arr1 || !s2)
		return (NULL);
	size_all = len1 + 1;
	res = malloc(sizeof(char *) * (size_all + 1));
	if (!res)
		return (NULL);
	res[size_all] = NULL;
	i = 0;
	while (i < len1)
	{
		res[i] = arr1[i];
		i++;
	}
	res[i] = s2;
	free(arr1);
	arr1 = NULL;
	return (res);
}
