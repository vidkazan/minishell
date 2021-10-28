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

int   builtin_fd_gen(t_elem *elem)
{
    if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
    {
        if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
            return 1;
        if(elem->data->double_redirect_output_fd)
            return elem->data->double_redirect_output_fd;
        if(elem->data->simple_redirect_output_fd)
            return elem->data->simple_redirect_output_fd;
    }
    else if(elem->type == CMD && !elem->prev && !elem->next) // first_CMD
    {
        if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
            return 1;
        if(elem->data->double_redirect_output_fd)
            return elem->data->double_redirect_output_fd;
        if(elem->data->simple_redirect_output_fd)
            return elem->data->simple_redirect_output_fd;
    }
    else if(elem->type == PIPE && !elem->prev && elem->next) // first_PIPE
        return elem->pfd[1];
    else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
        return elem->pfd[1];
    write(2, ">>> builtin_fd_gen: else!\n", 10);
        return 1;
}

void close_fd(t_elem *elem)
{
    if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
        close(elem->prev->pfd[0]);
    else if(elem->type == PIPE && !elem->prev) // first_PIPE
        close(elem->pfd[1]);
    else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
    {
        close(elem->prev->pfd[0]);
        close(elem->pfd[1]);
    }
}

void builtin_exec(t_elem *elem)
{
    int write_fd;
    write_fd = builtin_fd_gen(elem);
    if(!ft_strncmp(elem->cmd[0], "echo", 4)  && ft_strlen(elem->cmd[0]) == 4)
        builtin_echo(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "cd", 2)  && ft_strlen(elem->cmd[0]) == 2)
        builtin_cd(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "pwd", 3)  && ft_strlen(elem->cmd[0]) == 3)
        builtin_pwd(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "env", 3)  && ft_strlen(elem->cmd[0]) == 3)
    	builtin_env(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "exit", 4)  && ft_strlen(elem->cmd[0]) == 4)
        builtin_exit(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "unset", 5)  && ft_strlen(elem->cmd[0]) == 5)
        builtin_unset(elem, write_fd);
    if(!ft_strncmp(elem->cmd[0], "export", 6) && ft_strlen(elem->cmd[0]) == 6)
        builtin_export(elem, write_fd);
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
        builtin_check(elem);
        if(elem->data->debug && elem->is_builtin)
            dprintf(2, ">>> %d builtin\n", getpid());
        if(!elem->is_builtin)
            find_path(elem);
        if (elem->type == PIPE)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d pipe\n", getpid());
            if(pipe(elem->pfd))
            {
                write(2, strerror(errno), ft_strlen(strerror(errno)));
                return;
            }
        }
        if(!elem->is_builtin)
        {
            elem->pid = fork();
            if (elem->pid < 0)
            {
                write(2, strerror(errno), ft_strlen(strerror(errno)));
                return;
            }
        }
        if (elem->pid == 0 && !elem->is_builtin)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d child\n", getpid());
            if (elem->type == CMD && elem->prev && !elem->next) // last_CMD
            {
                dup2(elem->prev->pfd[0], 0);
                close(elem->prev->pfd[0]);
                close(elem->prev->pfd[1]);
                if(elem->data->double_redirect_output_fd)
                    dup2(elem->data->double_redirect_output_fd, 1);
                if(elem->data->simple_redirect_output_fd)
                    dup2(elem->data->simple_redirect_output_fd, 1);
//                if(elem->data->simple_redirect_output_fd < 0 && elem->data->double_redirect_output_fd < 0)
//                    dup2(elem->data->std_out, 1);
            }
            else if(elem->type == CMD && !elem->prev && !elem->next) // first_CMD
            {
                if(elem->data->double_redirect_output_fd)
                    dup2(elem->data->double_redirect_output_fd, 1);
                if(elem->data->simple_redirect_output_fd)
                    dup2(elem->data->simple_redirect_output_fd, 1);
                if(elem->data->simple_redirect_input_fd)
                    dup2(elem->data->simple_redirect_input_fd, 0);
            }
            else if(elem->type == PIPE && !elem->prev) // first_PIPE
            {
                if(elem->data->simple_redirect_input_fd)
                    dup2(elem->data->simple_redirect_input_fd, 0);
                dup2(elem->pfd[1], 1);
                close(elem->pfd[0]);
                close(elem->pfd[1]);
            }
            else if(elem->type == PIPE && elem->prev && elem->next) // middle_PIPE
            {
                dup2(elem->pfd[1], 1);
                dup2(elem->prev->pfd[0], 0);
                close(elem->pfd[0]);
                close(elem->pfd[1]);
                close(elem->prev->pfd[0]);
                close(elem->prev->pfd[1]);
            }
            if (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0)
            {
                ft_putstr_fd(strerror(errno), 2);
                exit(1);
            }
        }
        else
        {
            if(elem->is_builtin)
                builtin_exec(elem);
            if(elem->data->debug)
            dprintf(2, ">>> %d parent\n", getpid());
            close_fd(elem);
        }
        if (elem->next)
        {
            if(elem->data->debug)
            dprintf(2, ">>> %d next\n", getpid());
            elem = elem->next;
        }
        else
            break;
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
	    if(!elem->is_builtin)
            waitpid(elem->pid, 0,0);
		if(elem->next)
			elem = elem->next;
		else
            break;
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
            if(data->debug)
                print_elems(data->elem_start);
			data_reboot(data, NULL, 0);
		}
    }
    closing(data);
    return 0;
}
