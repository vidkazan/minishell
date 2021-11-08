//
// Created by fcody on 10/20/21.
//

#include "main.h"

void	free_arr(char **str)
{
    int	i;

    i = -1;
    while (str[++i])
        free(str[i]);
    free(str);
    str = NULL;
}

static int	access_granted(t_elem *elem, char *filename_with_path)
{
    if (access(filename_with_path, F_OK) > -1)
    {
//        if(elem->cmd[0])
//            free(elem->cmd[0]);
        elem->cmd[0] = filename_with_path;
        return (1);
    }
    return (0);
}

void	path_check(t_elem *elem, char **path_arr, char *file_no_path)
{
    char	*filename_with_path;
    char	*filename_with_slash;
    int		i;

    i = -1;
    while (path_arr[++i])
    {
        filename_with_slash = ft_strjoin(path_arr[i], "/");
        if (!filename_with_slash)
            return;
        filename_with_path = \
			ft_strjoin(filename_with_slash, file_no_path);
        free(filename_with_slash);
        filename_with_slash = NULL;
        if (!filename_with_path)
            return;
        if (access_granted(elem, filename_with_path) == 1)
            break ;
        free(filename_with_path);
        filename_with_path = NULL;
    }
}

void	env_path_find(t_data *data)
{
    int		i;

    i = -1;
    while (data->envp[++i])
    {
        if (!ft_strncmp(data->envp[i], "PATH=", 5))
            data->path = (data->envp[i] + 5);
    }
}

void	find_path(t_elem *elem)
{
    char	*filename_no_path;
    char	**path_arr;

    path_arr = ft_split(elem->data->path, ':');
    if (!path_arr)
        return;
    filename_no_path = elem->cmd[0];
    path_check(elem, path_arr, filename_no_path);
    free_arr(path_arr);
}


