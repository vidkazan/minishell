#include "main.h"

int make_new_elem(int start, int end, t_data *data)
{
	t_elem	*new;
	int		return_value;

	return_value = 0;
	if (!data->elem_start)
		new = push_back(new, data);
	else
		new = push_back(new, data);
	while(new->next)
		new = new->next;
	new->type = 1;
	new->cal = malloc(sizeof(char *) + sizeof(char) * (end - start + 1));
	if (data->line[end] == '|')
		new->type = 2;
	if (data->line[end] == '>')
	{
		new->type = 4;
		if (data->line[end + 1] && data->line[end + 1] == '>')
		{
			return_value = 1;
			new->type = 6;
		}
	}
	if (data->line[end] == '<')
	{
		new->type = 3;
		if (data->line[end + 1] && data->line[end + 1] == '<')
		{
			return_value = 1;
			new->type = 5;
		}
	}
	ft_strlcpy(new->cal, data->line + start, end - start + 1);
	(new->cal)[end - start + 1] = 0;
	new->cmd = ft_split(new->cal, ' ');
	if (!data->elem_start)
		data->elem_start = new;
	if(data->debug)
	    printf("%d %d | cmd=^^%s^^, type = %d\n", start, end, new->cal, new->type);
	return(return_value);
}

void	quotes(char c, t_data *data)
{
	if (c == '\'' && !data->q1)
		data->q2 = (data->q2 + 1) % 2;
	if (c == '\"' && !data->q2)
		data->q1 = (data->q1 + 1) % 2;
}

void	main_preparser(t_data *data, char *line, char **env)
{
	int prev_end;
	int i = 0;
	int	swift;

	prev_end = 0;
	while (line[i])
	{
		swift = 0;
		quotes(line[i], data);
		if ((line[i] == '|' || line[i] == '<' || line[i] == '>') && !data->q1 && !data->q2)
		{
			swift = make_new_elem(prev_end, i, data);
			prev_end = i + 1 + swift;
			i += swift;
		}
		i++;
	}
	make_new_elem(prev_end, i, data);
	if (data->q1 || data->q2)
		data_reboot(data, "Error: Unclosed quotes", 1);
	// t_elem	*temp = *(data->elem_start); // writing each element
	// while (temp)
	// {
	// 	printf("%s, type = %d\n", temp->str, temp->next_type);
	// 	temp = temp->next;
	// }
	// temp = *(data->list);
	// printf("_____________________\n\n");
	// if (temp)
	// 	parser(data);
}
