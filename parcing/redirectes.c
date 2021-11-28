#include "../main.h"

int	redir_find_end(char *str)
{
	int	num;
	int	i;

	num = 0;
	i = 1;
	if ((str[i - 1] == '>' && str[i] == '>') || \
		(str[i - 1] == '<' && str[i] == '<'))
		i++;
	while (str[i] == ' ' && str[i])
	{
		num++;
		i++;
	}
	while (str[i] != ' ' && str[i] != '>' && \
		str[i] != '<' && str[i] != '|' && str[i])
	{
		num++;
		i++;
	}
	return (num);
}

void	set_redirect_type(t_data *data, t_elem *new, int start)
{
	if (data->line[start] == '>')
	{
		new->type = 4;
		if (data->line[start + 1] && data->line[start + 1] == '>')
			new->type = 6;
	}
	if (data->line[start] == '<')
	{
		new->type = 3;
		if (data->line[start + 1] && data->line[start + 1] == '<')
			new->type = 5;
	}
}

int	new_redirect_elem(int start, t_data *data)
{
	t_elem	*new;
	int		count;

	new = push_back(data->elem_start, data);
	data->elem_start = new;
	count = redir_find_end(data->line + start);
	set_redirect_type(data, new, start);
	new->str = ft_substr(data->line, start + 1 + (new->type > 4), count);
	new->cmd = shell_split(new->str, ' ');
	if (!new->cmd[0])
		return (1);
	if (new->type == 5 || new->type == 6)
		data->line[start++] = ' ';
	data->line[start] = ' ';
	while (count--)
		data->line[++start] = ' ';
	return (0);
}

int	make_redirect_elems(t_data *data, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		quotes(i, data);
		if ((line[i] == '<' || line[i] == '>') && !data->q1 && !data->q2)
		{
			if (new_redirect_elem(i, data))
			{
				builtins_error(data, NULL, NULL, \
					"syntax error after redirect token");
				data->exit_status = 2;
				data->exec = 0;
				return (1);
			}
		}
		i++;
	}
	return (0);
}
