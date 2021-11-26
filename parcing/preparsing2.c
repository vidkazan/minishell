#include "../main.h"

int new_pipe_elem(int start, int end, t_data *data)
{
    t_elem	*new;

    new = push_back(data->elem_start, data);
    data->elem_start = new;
    while(new->next)
        new = new->next;
    new->type = 1;
    new->comand_line = malloc(sizeof(char *) + sizeof(char) * (end - start + 1));
    if (data->line[end] == '|')
        new->type = 2;
    ft_strlcpy(new->comand_line, data->line + start, end - start + 1);
    (new->comand_line)[end - start + 1] = 0;
    new->cmd = shell_split(new->comand_line, ' ');
    int i = 0;
    if(data->debug)
        printf("%d %d | cmd=^^%s^^, type = %d\n", start, end, new->comand_line, new->type);
    if (!*new->cmd)
        return(1);
    return(0);
}

void	quotes(int i, t_data *data)
{
    if ((data->line)[i] == '\'' && !data->q1)
        data->q2 = (data->q2 + 1) % 2;
    if ((data->line)[i] == '\"' && !data->q2)
        data->q1 = (data->q1 + 1) % 2;
}

int redir_find_end(char *str, int start, t_data *data)
{
    int	num;
    int	i;

    num = 0;
    i = 1;
    if ((str[i - 1] == '>' && str[i] == '>') || (str[i - 1] == '<' && str[i] == '<'))
        i++;
    while(str[i] == ' ' && str[i])
    {
        num++;
        i++;
    }
    while(str[i] != ' ' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])
    {
        num++;
        i++;
    }
    return (num);
}

void set_redirect_type(t_data *data, t_elem *new, int start)
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
    count = redir_find_end(data->line + start, start, data);
    set_redirect_type(data, new, start);
    new->comand_line = ft_substr(data->line, start + 1 + (new->type > 4), count);
    new->cmd = shell_split(new->comand_line, ' ');
    if (!new->cmd[0])
        return(1);
    if (new->type == 5 || new->type == 6)
        data->line[start++] = ' ';
    data->line[start] = ' ';
    while (count--)
        data->line[++start] = ' ';
    return(0);
}

int	make_redirect_elems(t_data *data, char *line)
{
    int	i = 0;

    while (line[i])
    {
        quotes(i, data);
        if ((line[i] == '<' || line[i] == '>') && !data->q1 && !data->q2)
        {
            if (new_redirect_elem(i, data))
            {
        		builtins_error(data,NULL,NULL,"syntax error after redirect token", 2);
        		data->exec = 0;
                return (1);
            }
        }
        i++;
    }
	return(0);
}

void	del_last_elem(t_data *data)
{
    t_elem *elem_to_del;
    t_elem *prelast_elem;
    int count = 0;

    elem_to_del = data->elem_start;
    while (elem_to_del->next)
    {
        count++;
        elem_to_del = elem_to_del->next;
    }
	data->elem_start = delete_current_node(elem_to_del);
}

int check_last_cmd(t_data *data)
{
    t_elem *prelast;
    t_elem *last;
    last = data->elem_start;
    while(last->next)
        last = last->next;
    prelast = last->prev;
    if (prelast->type > 2)
    {
        free(last);
        prelast->next = NULL;
        return 0;
    }
    return(1);
}

int check_last_cmd(t_data *data)
{
    t_elem *prelast;
    t_elem *last;
    last = data->elem_start;
    while(last->next)
        last = last->next;
    prelast = last->prev;
    if (prelast->type > 2)
    {
        free(last);
        prelast->next = NULL;
        return 0;
    }

    // printf("type last cmd = %d\n", prelast->type);
    return(1);
}

int	main_preparser(t_data *data, char *line)
{
    int prev_end;
    int i = 0;

    prev_end = 0;
    data->elem_start = NULL;
    if (make_redirect_elems(data, line))
		return 1;
    while (line[i])
    {
        quotes(i, data);
        if (line[i] == '|' && !data->q1 && !data->q2)
        {
            if(new_pipe_elem(prev_end, i, data))
            {
        		builtins_error(data,NULL,NULL,"syntax error near unexpected token '|'", 2);
        		data->exec = 0;
        		return 1;
        	}
            prev_end = i + 1;
        }
        i++;
    }
    if (new_pipe_elem(prev_end, i, data))
	{
        if (check_last_cmd(data))
		{
            builtins_error(data,NULL,NULL,"syntax error no cmd", 2);
		    data->exec = 0;
    		return 1;
        }
    }
    // printf("|%s|\n", data->elem_start->comand_line);
    while(data->elem_start->prev)
        data->elem_start = data->elem_start->prev;
    if (data->q1 || data->q2)
	{
		builtins_error(data,NULL,NULL,"Error: Unclosed quotes", 2);
		data->exec = 0;
		return 1;
	}
	return(0);
}