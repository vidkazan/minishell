//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_pwd(t_elem *elem)
{
    char *pwd = getcwd(NULL, 1024);
    if(!pwd)
    {
		dprintf(2, "pwd:getcwd: error");
		return;
	}
    dprintf(2,"pwd: %s\n", pwd);
    free(pwd);
}

void builtin_cd(t_elem *elem) // no handling deleted dir error
{
	char *home;
	char *res_path;
	char *current_pwd_env;
	char *old_pwd_env;
	int	pwd_index = 0;
	int	old_pwd_index = 0;
	int index;

	current_pwd_env = ft_strdup(search_strings_in_array(elem->data->envp, "PWD=", &pwd_index));
	old_pwd_env = search_strings_in_array(elem->data->envp, "OLDPWD=", &old_pwd_index);
    if(!elem->cmd[1])
    {
    	res_path = ft_strdup(search_strings_in_array(elem->data->envp, "HOME=", &index));
        if(chdir(res_path))
        {
        	write(1, "error: chdir\n", 13);
        	return;
        }
    }
    else if(elem->cmd[1][0] == '~')
    {
    	elem->cmd[1]++;
		res_path = ft_strjoin(home, elem->cmd[1]);
		if(chdir(res_path))
		{
			write(1, "error: chdir\n", 13);
			return;
		}
	}
    else if(elem->cmd[1][0] == '-' && old_pwd_env)
    {
    	res_path = ft_strdup(old_pwd_env);
    	if(chdir(res_path))
    	{
    		write(1, "error: chdir\n", 13);
    		return;
    	}
    	else
    	{
			write(elem->data->std_out, res_path, ft_strlen(res_path));
			write(1, "\n", 1);
		}
    }
    else
    {
    	res_path = ft_strdup(elem->cmd[1]);
    	if(getcwd(NULL, 1024) || (ft_strncmp(elem->cmd[1], ".", 1) && ft_strlen(elem->cmd[1]) == 1))
    	{
			if (chdir(res_path))
			{
				write(1, "error: chdir\n", 13);
				return;
			}
		}
    	else
    	{
    		dprintf(2, "cd:error: no directory\n");
    	}
    }
	edit_env_keys(pwd_index, getcwd(NULL, 1024), elem->data);
    edit_env_keys(old_pwd_index, current_pwd_env, elem->data);
	builtin_pwd(elem);
    free(res_path);
    free(current_pwd_env);
}