#include "../main.h"

// int redirectNullAfterWord(t_elem *new)
// {
// 	char *str;

// 	str = new->comand_line;
// 	while (str)
// }

// void	del_outer_quotes(char *line)
// {
//     int i = 0;
//     int j = 0;
//     int q1 = 0;
//     int q2 = 0;
//     int flag =0;

//     while (line[i])
//     {
//         if (line[i] == '\'' && !q2)
//         {
//             q1 += 1;
//             q1 %= 2;
//             flag = 1;
//         }
//         if (line[i] == '\"' && !q1)
//         {
//             q2 += 1;
//             q2 %= 2;
//             flag = 1;
//         }
//         line[j] = line[i];
//         // printf("q1 flag = %d, char = %c\n", q1, line[i]);
//         if (!flag)
//             j++;
//         i++;
//         flag = 0;
//     }
//     line[j] = 0;
//     // printf("\n");
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
    new->comand_line = malloc(sizeof(char *) + sizeof(char) * (end - start + 1));
    if (data->line[end] == '|')
        new->type = 2;
    ft_strlcpy(new->comand_line, data->line + start, end - start + 1);
    // del_outer_quotes(new->comand_line);
    // if (new->type > 2)
    // redirectNullAfterWord(new);
    (new->comand_line)[end - start + 1] = 0;
    new->cmd = shell_split(new->comand_line, ' ');
    int i = 0;
    // while (new->cmd[i])
        // i++;
    // printf("split i = %d\n", i);
    // if (!data->elem_start)
    // data->elem_start = new;
    if(data->debug)
        printf("%d %d | cmd=^^%s^^, type = %d\n", start, end, new->comand_line, new->type);
    if (!*new->cmd)
        return_value = 1;
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
    t_elem	*new;
    int		count;

    new = push_back(data->elem_start, data);
    data->elem_start = new;
    count = redir_find_end(data->line + start, start, data);
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
    new->comand_line = ft_substr(data->line, start + 1 + (new->type > 4), count);
    // del_outer_quotes(new->comand_line);
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
    // int	count;

    while (line[i])
    {
        // count = 0;
        quotes(i, data);
        if ((line[i] == '<' || line[i] == '>') && !data->q1 && !data->q2)
        {
            if (new_redirect_elem(i, data))
				return(1);
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
    if (count)
    {
        prelast_elem = elem_to_del->prev;
    }
    free(elem_to_del);
    if (count)
        prelast_elem->next = NULL;
}

int	main_preparser(t_data *data, char *line)
{
    int prev_end;
    int i = 0;

    prev_end = 0;
    data->elem_start = NULL;
    if (make_redirect_elems(data, line))
//		return(data_reboot(data, "syntax error near unexpected token `newline'", 1));
	{
		builtins_error(data,NULL,NULL,"syntax error near unexpected token `newline'", 2);
		data->exec = 0;
		return 1;
	}
    while (line[i])
    {
        quotes(i, data);
        if (line[i] == '|' && !data->q1 && !data->q2)
        {
            new_pipe_elem(prev_end, i, data);
            prev_end = i + 1;
        }
        i++;
    }
    if (new_pipe_elem(prev_end, i, data))
        del_last_elem(data);
    // printf("|%s|\n", data->elem_start->comand_line);
    while(data->elem_start->prev)
        data->elem_start = data->elem_start->prev;
    // printf("%p\n", data->elem_start);
    if (data->q1 || data->q2)
	{
		builtins_error(data,NULL,NULL,"Error: Unclosed quotes", 2);
		data->exec = 0;
		return 1;
	}
	return(0);

}