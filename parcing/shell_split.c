#include "../main.h"

void	del_outer_quotes(char *line)
{
    int i = 0;
    int j = 0;
    int q1 = 0;
    int q2 = 0;
    int flag =0;

    while (line[i])
    {
        if (line[i] == '\'' && !q2)
        {
            q1 += 1;
            q1 %= 2;
            flag = 1;
        }
        if (line[i] == '\"' && !q1)
        {
            q2 += 1;
            q2 %= 2;
            flag = 1;
        }
        line[j] = line[i];
        // printf("q1 flag = %d, char = %c\n", q1, line[i]);
        if (!flag)
            j++;
        i++;
        flag = 0;
    }
    line[j] = 0;
    // printf("\n");
}

int	ft_value_of_strings(char c, t_data *data)
{
	int word_flag;
	int	num;
	int	i;

	num = 0;
	i = -1;
	word_flag = 0;
	while (data->line[++i])
	{
		if (data->line[i] == '\'' || data->line[i] == '\"')
		{
			// if (!word_flag)
			// {
			// 	word_flag++;
			// 	num++;
			// }
			quotes(i, data);
		}
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
	// printf("val = %d\n", num);
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

char	**ft_write_arr(int value_str, t_data *data, char c, char **arr)
{
	int	i;
	int	n_str;
	int	n_sum;
	char *s = data->line;

	n_str = 0;
	i = 0;
	while (n_str < value_str)
	{
		n_sum = 0;
		while ((s[i] != c || data->q1 || data->q2) && s[i])
		{
			quotes(i, data);
			i++;
			n_sum++;
		}
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

char	**shell_split(char const *s, char c)
{
	t_data data;
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

// int main(void)
// {
// 	char *line;
// 	char **array;
// 	int i;

// 	while(1)
// 	{
// 		line = readline("enter line:");
// 		array = shell_split(line, ' ');
// 		free(line);
// 		line = NULL;
// 		i = 0;
// 		while(array[i])
// 		{
// 			printf("%d:|%s|\n", i, array[i++]);
// 		}
// 		printf("-------------------------------\n");
// 	}
// }