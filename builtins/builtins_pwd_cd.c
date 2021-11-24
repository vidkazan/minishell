//
// Created by fcody on 10/21/21.
//

#include "../main.h"

void builtin_pwd(t_elem *elem, int write_fd)
{
    int i = 0;
    char *res;
//    res = search_strings_in_array(elem->data->envp, "PWD=", &i);
//    if(!res)
//        builtins_exit_status(elem, "pwd",NULL, "no env PATH");
//    else
//        ft_putendl_fd(res, write_fd);
    res = getcwd(NULL, 0);
    if(!res)
        builtins_error(elem->data, "pwd", NULL, NULL, 0);
    else
        ft_putendl_fd(res, write_fd);
}

void builtin_cd(t_elem *elem,int write_fd) // relative path cd NOT working if current dir is removed
{
	char *home;
	char *res_path;
	char *current_pwd_env;
	int	pwd_index = 0;
	int	old_pwd_index = 0;
	home = search_strings_in_array(elem->data->envp, "HOME=", NULL);
	search_strings_in_array(elem->data->envp, "OLDPWD=", &old_pwd_index);
    if(!elem->cmd[1])
    {
        if(!home || !*home)
            builtins_error(elem->data, "cd",NULL, "HOME not set", 1);
        else if(chdir(home))
            builtins_error(elem->data, "cd",NULL, NULL, 0);
        return;
    }
    if(!elem->cmd[1][0])
        return;
    else if(elem->cmd[1][0] == '~')
    {
        elem->cmd[1]++;
		res_path = ft_strjoin(home, elem->cmd[1]);
		if(chdir(res_path))
		{
            builtins_error(elem->data, "cd",NULL, NULL, 0);
			return;
		}
	}
    else
    {
        res_path = ft_strdup(elem->cmd[1]);
        if(!getcwd(NULL, 0))
         {
             builtins_error(elem->data, "cd",elem->cmd[1], NULL, 0);
             return;
         }
        if (chdir(res_path))
        {
            builtins_error(elem->data, "cd",elem->cmd[1], NULL, 0);
            return;
        }
    }
    current_pwd_env = ft_strdup(search_strings_in_array(elem->data->envp, "PWD=", &pwd_index));
	edit_env_keys(pwd_index, getcwd(NULL, 1024), elem->data);
    edit_env_keys(old_pwd_index, current_pwd_env, elem->data);
    free(res_path);
    free(current_pwd_env);
}