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
#include <stdlib.h>

int ft_strlen_arr(char **arr)
{
    int i;

    i = 0;
    if(!arr || !arr[0])
        return 0;
    while(arr[i])
        i++;
    return i;
}

char    **ft_arrdup(char **env)
{
    char **arr_dup;
    int i = -1;
    int arr_len = ft_strlen_arr(env);

    if(!env || !env[0])
        return NULL;
    arr_dup = (char **)malloc(sizeof(char *) * (arr_len + 1));
    while (++i < arr_len)
        arr_dup[i] = ft_strdup(env[i]);
    arr_dup[arr_len] = NULL;
    return arr_dup;
}

char *search_strings_in_array(char **arr, char *search_word, int *index)
{
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
