#include "../main.h"

void builtin_cd_pwd_oldpwd(t_elem *elem)
{
	char *current_pwd_env;
	int	old_pwd_index;
	int	pwd_index;

	pwd_index= 0;
	old_pwd_index= 0;
	search_strings_in_array(elem->data->envp, "OLDPWD", &old_pwd_index, 1);
	current_pwd_env = ft_strdup(search_strings_in_array(elem->data->envp, "PWD", &pwd_index, 1));
	edit_env_keys(pwd_index, getcwd(NULL, 0), elem->data);
	edit_env_keys(old_pwd_index, current_pwd_env, elem->data);
}

void builtin_pwd(t_elem *elem, int write_fd)
{
    char *res;
	{
		res = getcwd(NULL, 0);
		if(!res)
			builtins_error(elem->data, "pwd", NULL, NULL);
		else
			ft_putendl_fd(res, write_fd);
		free(res);
	}
}

void builtin_cd_empty(t_elem *elem)
{
	char *home;

	home = search_strings_in_array(elem->data->envp, "HOME", NULL, 1);
	if(!home || !*home)
		builtins_error(elem->data, "cd",NULL, "HOME not set");
	else if(chdir(home))
		builtins_error(elem->data, "cd", NULL, NULL);
	builtin_cd_pwd_oldpwd(elem);
}

void builtin_cd_main(t_elem *elem)
{
	char *getcwd_res;
	char *res_path;

	res_path = ft_strdup(elem->cmd[1]);
	getcwd_res = getcwd(NULL, 0);
	if(!getcwd_res && !ft_strcmp(res_path, "."))
	{
		free(getcwd_res);
		builtins_error(elem->data, "getcwd",res_path, NULL);
		free(res_path);
		return;
	}
	if (chdir(res_path))
	{
		free(getcwd_res);
		builtins_error(elem->data, "cd",elem->cmd[1], NULL);
		free(res_path);
		return;
	}
	free(getcwd_res);
	free(res_path);
	builtin_cd_pwd_oldpwd(elem);
}

void builtin_cd(t_elem *elem)
{
    if(!elem->cmd[1] || !elem->cmd[1][0])
		builtin_cd_empty(elem);
	else if(elem->cmd[1][0] == '~')
		builtins_error(elem->data, "cd","~", "atata tak nel\'zya");
    else
		builtin_cd_main(elem);
}