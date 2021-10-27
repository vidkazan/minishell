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

void close_fd(t_elem *elem)
{
    if(elem->data->debug)
        dprintf(2, ">>> %d closing fd\n", elem->pid);
	if(elem->prev && elem->prev->pfd[0] > 2)
		close(elem->prev->pfd[0]);
	if(elem->pfd[1] != -1)
		close(elem->pfd[1]);
}

void builtin_exec(t_elem *elem)
{
    if(!ft_strncmp(elem->cmd[0], "echo", 4))
        builtin_echo(elem);
    if(!ft_strncmp(elem->cmd[0], "cd", 2))
        builtin_cd(elem);
    if(!ft_strncmp(elem->cmd[0], "pwd", 3))
        builtin_pwd(elem);
    if(!ft_strncmp(elem->cmd[0], "env", 3))
    	builtin_env(elem);
    if(!ft_strncmp(elem->cmd[0], "exit", 4))
        builtin_exit(elem);
    if(!ft_strncmp(elem->cmd[0], "unset", 5))
        builtin_unset(elem);
}

void builtin_check(t_elem *elem)
{
    if(!ft_strncmp(elem->cmd[0], "echo", 4) || !ft_strncmp(elem->cmd[0], "cd", 2) || \
    !ft_strncmp(elem->cmd[0], "pwd", 3) || !ft_strncmp(elem->cmd[0], "export", 6) || \
    !ft_strncmp(elem->cmd[0], "unset", 5) || !ft_strncmp(elem->cmd[0], "env", 3) || \
    !ft_strncmp(elem->cmd[0], "exit", 4))
        elem->is_builtin = 1;
}

void execution(t_elem *elem)
{
    while(elem)
    {
        if(elem->data->debug)
        	dprintf(2,"\n>>> %.05d: now %p\n", elem->pid, elem);
        builtin_check(elem);
        if(!elem->is_builtin)
            find_path(elem);
        if(elem->data->debug)
        	dprintf(2,">>> %.05d: %s \n", elem->pid, elem->cmd[0]);
		if (elem->type == CMD && elem->prev && !elem->next)
        {
            if(elem->data->debug)
            	dprintf(2,">>> %.05d: last_cmd\n", elem->pid);
            dup2(elem->prev->pfd[0], 0);
            if(elem->data->double_redirect_output_fd)
				dup2(elem->data->double_redirect_output_fd, 1);
			if(elem->data->simple_redirect_output_fd)
				dup2(elem->data->simple_redirect_output_fd, 1);
            if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
                dup2(elem->data->std_out, 1);
        }
        else if(elem->type == CMD && !elem->prev && !elem->next)
        {
            if(elem->data->debug)
            	dprintf(2,">>> %.05d: first_cmd\n", elem->pid);
			if(elem->data->double_redirect_output_fd)
				dup2(elem->data->double_redirect_output_fd, 1);
			if(elem->data->simple_redirect_output_fd)
				dup2(elem->data->simple_redirect_output_fd, 1);
			if(elem->data->simple_redirect_input_fd)
				dup2(elem->data->simple_redirect_input_fd, 0);
		}
        if (elem->type == PIPE)
        {
            if(elem->data->debug)
            	dprintf(2,">>> %.05d: pipe\n", elem->pid);
            pipe(elem->pfd);
            if(!elem->prev)
            {
            	if(elem->data->simple_redirect_input_fd)
            		dup2(elem->data->simple_redirect_input_fd, 0);
                if(elem->data->debug)
                	dprintf(2,">>> %.05d: first_pipe\n", elem->pid);
                dup2(elem->pfd[1], 1);
            }
            else if(elem->prev && elem->next)
            {
                if(elem->data->debug)
                	dprintf(2,">>> %.05d: middle_pipe\n", elem->pid);
                dup2(elem->pfd[1], 1);
                dup2(elem->prev->pfd[0], 0);
            }
        }
        if(!elem->is_builtin)
        {
            if(elem->data->debug)
            	dprintf(2,">>> %.05d: fork\n", elem->pid);
            elem->pid = fork();
            if (elem->pid == 0)
            {
                if (elem->data->debug)
                	dprintf(2, ">>> %.05d: child\n", elem->pid);
                if (!elem->is_builtin && (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0))
                {
					ft_putstr_fd(strerror(errno), 2);
					exit(1);
				}
            }
        }
        else
        {
            if(elem->data->debug)
            	dprintf(2,">>> %.05d: builtin\n", elem->pid);
            builtin_exec(elem);
        }
        if(elem->pid != 0)
        {
            close_fd(elem);
            if (elem->data->debug)
                dprintf(2, ">>> %.05d: end of %p\n", elem->pid, elem);
            if (elem->next)
            {
                if (elem->data->debug)
                    dprintf(2, ">>> %.05d: moving to %p\n", elem->pid, elem->next);
                elem = elem->next;
            }
            else
                break;
        }
    }
}

void init(t_data *data, char **env)
{
//    data->envp = ft_arrdup(env);
    data->std_in = dup(0);
    data->std_out = dup(1);
    data->error = 0;
    data->elem_start = NULL;
    data->simple_redirect_input_fd = -1;
    data->simple_redirect_output_fd = -1;
    data->double_redirect_output_fd = -1;
//    env_path_find(data);
	data->q1 = 0;
	data->q2 = 0;
}

void waiting(t_data *data)
{
	t_elem *elem = data->elem_start;
	while (elem)
	{
        if(data->debug)
            printf(">>> %d wait %s\n",elem->pid, elem->cmd[0]);
        if(elem->prev)
		kill(elem->prev->pid,1);
        wait(0);
        if(data->debug)
        printf(">>> %d killed %s\n",elem->pid, elem->cmd[0]);
		if(elem->next)
			elem = elem->next;
		else
		{
            if(data->debug)
            printf(">>> %d kill end s\n",elem->pid);
            break;
        }
	}
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

void	data_reboot(t_data *data, char *message, int mode)
{
	if (mode)
		printf("%s\n", message);
	list_cleaner(data->elem_start);
	init(data, data->envp);		//	need to init everytime?
}


void closing(t_data *data)
{
    free_arr(data->envp);
}

void	read_line_and_add_history(char **line)
{
	if (*line)
	{
		free(*line);
		*line = (char *)NULL;
	}
	*line = readline("minishell $> ");
	if (*line && **line)
		add_history (*line);
}

int main(int ac, char **av, char **env)
{
    t_data *data = malloc(sizeof (t_data));
	int		exit_flag = 0;

    init(data, env);
    data->envp = ft_arrdup(env);
    env_path_find(data);
    if(ac == 2)
        data->debug = 1;
	while (!exit_flag)
	{
		read_line_and_add_history(&data->line);
		// ft_strip(&line);
		if (data->line && !ft_strncmp(data->line, "exit", 5))
			exit_flag = 1;
		else
		{
			main_preparser(data, data->line, env);
            simple_redirects(data);
            execution(data->elem_start);
            waiting(data);
            dup2(data->std_out,1);
            dup2(data->std_in,0);
//             print_elems(data->elem_start);
			data_reboot(data, NULL, 0);
		}
    }
    closing(data);
    return 0;
}