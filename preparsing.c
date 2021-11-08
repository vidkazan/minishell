#include "main.h"

// int redirectNullAfterWord(t_elem *new)
// {
// 	char *str;

// 	str = new->cal;
// 	while (str)
// }

int new_pipe_elem(int start, int end, t_data *data)
{
    t_elem	*new;
    int		return_value;

    return_value = 0;
    new = push_back(data->elem_start, data);
    data->elem_start = new;
    while(new->next)
        new = new->next;
    new->type = 1;
    new->cal = malloc(sizeof(char *) + sizeof(char) * (end - start + 1));
    if (data->line[end] == '|')
        new->type = 2;
    ft_strlcpy(new->cal, data->line + start, end - start + 1);

    // if (new->type > 2)
    // redirectNullAfterWord(new);
    (new->cal)[end - start + 1] = 0;
    new->cmd = ft_split(new->cal, ' ');
    // if (!data->elem_start)
    // data->elem_start = new;
    if(data->debug)
        printf("%d %d | cmd=^^%s^^, type = %d\n", start, end, new->cal, new->type);
    return(return_value);
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
    // int flag = 0;

    num = 0;
    i = 1;
    if ((str[i - 1] == '>' && str[i] == '>') || (str[i - 1] == '<' && str[i] == '<'))
    {
        // printf("%c->1\n",str[i]);
        i++;
    }
    // printf("str[i] = %c\n", str[i]);
    while(str[i] == ' ' && str[i])		// заменить нижнее подчеркивание на пробел
    {
        // printf("%c->2\n",str[i]);
        num++;
        i++;
    }
    while(str[i] != ' ' && str[i] != '>' && str[i] != '<' && str[i] != '|' && str[i])		// заменить нижнее подчеркивание на пробел
    {
        // printf("%c->3\n",str[i]);
        num++;
        // flag = 1;
        i++;
    }
    // printf("%d, %s\n", i, str);
    return (num);
}



int	new_redirect_elem(int start, t_data *data)
{
    // int		swift;
    t_elem	*new;
    int		count;

    // swift = 0;
    new = push_back(data->elem_start, data);
    data->elem_start = new;
    while(new->next)
        new = new->next;
    // new->type = 1;
    count = redir_find_end(data->line + start, start, data);
    if (data->line[start] == '>')
    {
        new->type = 4;
        if (data->line[start + 1] && data->line[start + 1] == '>')
        {
            // swift = 1;
            new->type = 6;
        }
    }
    if (data->line[start] == '<')
    {
        new->type = 3;
        if (data->line[start + 1] && data->line[start + 1] == '<')
        {
            // swift = 1;
            new->type = 5;
        }
    }
    new->cal = ft_substr(data->line, start + 1 + (new->type > 4), count);
    int i = 0;
    new->cmd = ft_split(new->cal, ' ');
    // while (new->cmd[i])
    // printf("redirect cmd:%s\n", new->cmd[i++]);
    // printf("%d\n", count);
    // exit(1);
    if (new->type == 5 || new->type == 6)
    {
        data->line[start++] = ' ';
    }
    data->line[start] = ' ';
    while (count--)
    {
        // printf("1%d, %d\n", count, start);
        data->line[++start] = ' ';
    }
    return(1);
}

void	make_redirect_elems(t_data *data, char *line)
{
    int i = 0;
    int	count;

    while (line[i])
    {
        count = 0;
        quotes(i, data);
        if ((line[i] == '<' || line[i] == '>') && !data->q1 && !data->q2)
        {
            // printf("%C%C%c\n", line[i], line[i + 1], line[i + 2]);
            count = new_redirect_elem(i, data);
            // printf("count = %d, start = %c, end = %c\n", count, line[i], line[i + count]);
            // i += count;
        }
        i++;
    }
}

void	main_preparser(t_data *data, char *line)
{
    int prev_end;
    int i = 0;

    prev_end = 0;
    data->elem_start = NULL;
    make_redirect_elems(data, line);
    // printf("///%s\n", data->line);
    while (line[i])
    {
        quotes(i, data);
        if (line[i] == '|'/* || line[i] == '<' || line[i] == '>') */ && !data->q1 && !data->q2)
        {
            new_pipe_elem(prev_end, i, data);
            prev_end = i + 1;
        }
        i++;
    }
    new_pipe_elem(prev_end, i, data);
    if (data->q1 || data->q2)
        data_reboot(data, "Error: Unclosed quotes", 1);
}