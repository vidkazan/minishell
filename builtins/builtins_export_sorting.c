#include "../main.h"

void	export_print(int write_fd, char **export_arr, int count)
{
	int i;

	i = -1;
	while (export_arr[++i])
	{
		count = -1;
		ft_putstr_fd("declare -x ", write_fd);
		while(export_arr[i][++count] && export_arr[i][count] != '=')
			ft_putchar_fd(export_arr[i][count], write_fd);
		if(export_arr[i][count] == '=')
		{
			ft_putstr_fd("=\"", write_fd);
			while(export_arr[i][++count])
				ft_putchar_fd(export_arr[i][count], write_fd);
			ft_putstr_fd("\"", write_fd);

		}
		ft_putchar_fd('\n', write_fd);
	}
	free(export_arr);
	export_arr = NULL;
}

void	export_sort(t_elem *elem, int write_fd)
{
    int i;
    int count;
    int env_arr_len;
    char **export_arr;

	count = -1;
    env_arr_len  = ft_strlen_arr(elem->data->envp);
    export_arr = ft_calloc(sizeof (char *), env_arr_len + 1);
    while(++count < env_arr_len)
    {
        i = -1;
        while(elem->data->envp[++i])
        {
            if(!is_in_export_line(elem->data->envp[i], export_arr) && is_min_arr_line(elem->data->envp[i], elem->data->envp, export_arr))
            {
                export_arr[count] = elem->data->envp[i];
                break;
            }
        }
    }
	export_print(write_fd, export_arr, count);
}

int		export_argument_check(char *arg)
{
    char *ptr;

    ptr = arg;
    if((!ptr || !*ptr) || (!ft_isalpha(*ptr) && *ptr != '_'))
        return 0;
	if(ptr[0] == '=')
		return 0;
    while(*ptr)
    {
        if(!ft_isalpha(*ptr) && !ft_isdigit(*ptr) && *ptr != '_' && *ptr != '=' && *ptr != '\'' && *ptr != '\"' && *ptr != ' ' && *ptr != '-')
            return 0;
        ptr++;
    }
    return 1;
}