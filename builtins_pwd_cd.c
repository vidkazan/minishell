//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_pwd(t_elem *elem, int write_fd)
{
    int i = 0;
    char *res;
    res = search_strings_in_array(elem->data->envp, "PWD=", &i);
    if(!res)
    {
        ft_putstr_fd("pwd:error: no env PATH", 2);
        return;
    }
    ft_putendl_fd(res, write_fd);

}

void builtin_cd(t_elem *elem,int write_fd) // no handling deleted dir error
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
    if(!elem->cmd[1][0])
        return;
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
//        (ft_strncmp(elem->cmd[1], ".", 1) &&
                 res_path = ft_strdup(elem->cmd[1]);
    	if(getcwd(NULL, 1024) ||ft_strlen(elem->cmd[1]) == 1)
    	{
			if (chdir(res_path))
			{
                dprintf(2, "getcwd:error: no directory\n");
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
//	builtin_pwd(elem);
    free(res_path);
    free(current_pwd_env);
}