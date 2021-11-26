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
//        builtins_error(elem->data, elem->cmd[0],NULL, "no env PATH", 0);
//    else
//        ft_putendl_fd(res, write_fd);
	{
		res = getcwd(NULL, 0);
		if(!res)
			builtins_error(elem->data, "pwd", NULL, NULL, 0);
		else
			ft_putendl_fd(res, write_fd);
		free(res);
	}

}

void builtin_cd(t_elem *elem,int write_fd) // relative path cd NOT working if current dir is removed
{
	char *getcwd_res;
	char *home;
	char *res_path;
	char *current_pwd_env;
	int	pwd_index = 0;
	int	old_pwd_index = 0;
	char *tilda_ptr = NULL;

	home = search_strings_in_array(elem->data->envp, "HOME", NULL, 1);
	search_strings_in_array(elem->data->envp, "OLDPWD", &old_pwd_index, 1);
    if(!elem->cmd[1] || !elem->cmd[1][0])
    {
        if(!home || !*home)
            builtins_error(elem->data, "cd",NULL, "HOME not set", 1);
        else if(chdir(home))
			builtins_error(elem->data, "cd", NULL, NULL, 0);
        return;
    }
    else if(elem->cmd[1][0] == '~')
    {
    	tilda_ptr = elem->cmd[1] + 1;
		res_path = ft_strjoin(home, tilda_ptr);
		if(chdir(res_path))
		{
			free(res_path);
            builtins_error(elem->data, "cd",NULL, NULL, 0);
			return;
		}
		free(res_path);
	}
    else
    {
		dprintf(2, ">>> here\n");
        res_path = ft_strdup(elem->cmd[1]);
        getcwd_res = getcwd(NULL, 0);
		dprintf(2, ">>> here1\n");
        if(!getcwd_res && !ft_strcmp(res_path, "."))
         {
			 free(getcwd_res);
			 dprintf(2, ">>> here2\n");
             builtins_error(elem->data, "getcwd",res_path, NULL, 0);
			 free(res_path);
             return;
         }
        if (chdir(res_path))
        {
			dprintf(2, ">>> here3\n");
            builtins_error(elem->data, "cd",elem->cmd[1], NULL, 0);
			free(res_path);
            return;
        }
		dprintf(2, ">>> here4\n");
		free(getcwd_res);
		free(res_path);
    }
	dprintf(2, ">>> here5\n");
    current_pwd_env = ft_strdup(search_strings_in_array(elem->data->envp, "PWD", &pwd_index,1));
	edit_env_keys(pwd_index, getcwd(NULL, 0), elem->data);
	dprintf(2, ">>> here6\n");
    edit_env_keys(old_pwd_index, current_pwd_env, elem->data);
}