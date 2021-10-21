//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_pwd(t_elem *elem)
{
    char *pwd = getcwd(NULL, 1000);
    write(1, pwd , ft_strlen(pwd));
    write(1, "\n", 1);
    free(pwd);
}
char *search_strings_in_array(char **arr, char *search_word)
{
	int i = -1;

	while(arr[++i])
	{
		if(!ft_strncmp(arr[i], search_word, ft_strlen(search_word)))
			return (arr[i] + ft_strlen(search_word));
	}
	return NULL;
}

void builtin_cd(t_elem *elem) // no handling deleted dir error / no writing PWD OLDPWD to ENV
{
	char *home;
	char *res_path;


    if(!elem->cmd[1])
    {
    	home = search_strings_in_array(elem->data->envp, "HOME=");
        if(chdir(home))
            write(1, "error: chdir\n", 13);
    }
    else if(elem->cmd[1][0] == '~')
    {
    	elem->cmd[1]++;
		res_path = ft_strjoin(home, elem->cmd[1]);
		if(chdir(res_path))
			write(1, "error: chdir\n", 13);
		free(res_path);
	}
    else if(elem->cmd[1][0] == '-')
    {
    	res_path = search_strings_in_array(elem->data->envp, "OLDPWD=");
    	if(chdir(res_path))
    		write(1, "error: chdir\n", 13);
    	else
    	{
			write(elem->data->std_out, res_path, ft_strlen(res_path));
			write(1, "\n", 1);
		}
    }
	builtin_pwd(elem);
}