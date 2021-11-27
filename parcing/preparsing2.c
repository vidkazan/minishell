#include "../main.h"

int	new_pipe_elem(int start, int end, t_data *data)
{
	t_elem	*new;

	new = push_back(data->elem_start, data);
	data->elem_start = new;
	while (new->next)
		new = new->next;
	new->type = 1;
	new->str = malloc(sizeof(char *) + (end - start + 1));
	if (data->line[end] == '|')
		new->type = 2;
	ft_strlcpy(new->str, data->line + start, end - start + 1);
	(new->str)[end - start + 1] = 0;
	new->cmd = shell_split(new->str, ' ');
	if (data->debug)
		printf("%d %d | cmd=^^%s^^, type = %d\n", \
			start, end, new->str, new->type);
	if (!*new->cmd)
		return (1);
	return (0);
}

void	quotes(int i, t_data *data)
{
	if ((data->line)[i] == '\'' && !data->q1)
		data->q2 = (data->q2 + 1) % 2;
	if ((data->line)[i] == '\"' && !data->q2)
		data->q1 = (data->q1 + 1) % 2;
}

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

int	check_last_cmd(t_data *data)
{
	t_elem	*prelast;
	t_elem	*last;

	last = data->elem_start;
	while (last->next)
		last = last->next;
	prelast = last->prev;
	if (prelast->type > 2)
	{
		delete_current_node(data,last);
		return (0);
	}
	return (1);
}

int	last_cmd_parser(t_data *data, int prev_end, int i)
{
	if (new_pipe_elem(prev_end, i, data))
	{
		while(data->elem_start->prev)
			data->elem_start = data->elem_start->prev;
//		print_elems(data->elem_start);
		if (check_last_cmd(data))
		{
			builtins_error(data, NULL, NULL, \
				"syntax error no cmd");
			data->exit_status = 2;
			data->exec = 0;
			return (1);
		}
	}
	while (data->elem_start->prev)
		data->elem_start = data->elem_start->prev;
	if (data->q1 || data->q2)
	{
		builtins_error(data, NULL, NULL, "Error: Unclosed quotes");
		data->exit_status = 2;
		data->exec = 0;
		return (1);
	}
	return (0);
}

int	main_preparser(t_data *data, char *line)
{
	if (data->debug)
		dprintf(2, ">>> %d parcer\n", getpid());
	int	prev_end;
	int	i;

	i = 0;
	prev_end = 0;
	data->elem_start = NULL;
	if (make_redirect_elems(data, line))
		return (1);
	while (line[i])
	{
		quotes(i, data);
		if (line[i] == '|' && !data->q1 && !data->q2)
		{
			if (new_pipe_elem(prev_end, i, data))
			{
				builtins_error(data, NULL, NULL, \
					"syntax error near unexpected token '|'");
			   	data->exit_status = 2;
				data->exec = 0;
				return (1);
			}
			prev_end = i + 1;
		}
		i++;
	}
	return (last_cmd_parser(data, prev_end, i));
}
