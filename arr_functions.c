#include "main.h"

//rl_clear_history, rl_on_new_line,
//rl_replace_line, rl_redisplay, add_history, printf,
//malloc, free, write, open, read, close, fork, wait,
//waitpid, wait3, wait4, signal, kill, exit, getcwd,
//chdir, stat, lstat, fstat, unlink, execve, dup,
//dup2, pipe, opendir, readdir, closedir, strerror,
//perror, isatty, ttyname, ttyslot, ioctl, getenv,
//tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum,
//tgetstr, tgoto, tputs

int ft_strlen_arr(char **arr)
{
//        dprintf(2, ">>> arrlen\n");
    int i;

    i = 0;
    if(!arr || !arr[0])
        return 0;
    while(arr[i])
        i++;
    return i;
}

char    **ft_arrdup(char **arr)
{
//        dprintf(2, ">>> arrdup\n");
    char **arr_dup;
    int i = -1;
    int arr_len = ft_strlen_arr(arr);

    if(!arr || !arr[0])
        return NULL;
    arr_dup = (char **)malloc(sizeof(char *) * (arr_len + 1));
    while (++i < arr_len)
        arr_dup[i] = ft_strdup(arr[i]);
    arr_dup[arr_len] = NULL;
    return arr_dup;
}


char	**ft_arrjoin(char **s1, char **s2)
{
    char	**res;
    int		size_all;
    int     i;
    int     len1;
    int     len2;

    len1 = ft_strlen_arr(s1);
    len2 = ft_strlen_arr(s2);
    if (!s1 || !s2)
        return (NULL);
    size_all = len1 + len2;
    res = malloc(sizeof(char*) * (size_all + 1));
    if (!res)
        return (0);
    res[size_all] = NULL;
    i = 0;
    while(i < len1)
    {
        res[i] = s1[i];
        i++;
    }
    while(i < size_all)
    {
        res[i] = s2[i - len1];
        i++;
    }
    return (res);
}

char *search_strings_in_array(char **arr, char *search_word, int *index)
{
//    dprintf(2, ">>> search_str_in_arr\n");
    int i = -1;

    if(!search_word)
        return NULL;
    while(arr[++i])
    {
        if(!ft_strncmp(arr[i], search_word, ft_strlen(search_word)))
        {
            if(index)
                *index = i;
            return (arr[i] + ft_strlen(search_word));
        }
    }
    return NULL;
}

void	ft_strip(char **str)
{
    int i;

    if (*str != NULL)
    {
        i = ft_strlen(*str);
        while ((*str)[i - 1] == ' ' && i > 0)
            i--;
        (*str)[i] = '\0';
        while (**str && **str == ' ')
            (*str)++;
    }
}

int	ft_strcmp(const char *s1, const char *s2)
{
    unsigned int	i;

    if(!s1 && !s2)
        return 0;
    else if(!s1 || !s2)
        return 1;
    i = 0;
    while (s1[i] == s2[i] && s1[i] && s2[i])
        i++;
    return (s1[i] - s2[i]);
}