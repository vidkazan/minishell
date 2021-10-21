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
	if(elem->prev)
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
            dprintf(2,">>> now %p\n", elem);
        builtin_check(elem);
        if(!elem->is_builtin)
            find_path(elem);
        if(elem->data->debug)
        dprintf(2,">>> %s \n", elem->cmd[0]);
		if (elem->type == CMD && elem->prev && !elem->next)
        {
            if(elem->data->debug)
            dprintf(2,">>> last_cmd\n");
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
			dprintf(2,">>> first_cmd\n");
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
            dprintf(2,">>> pipe\n");
            pipe(elem->pfd);
            if(!elem->prev)
            {
            	if(elem->data->simple_redirect_input_fd)
            		dup2(elem->data->simple_redirect_input_fd, 0);
                if(elem->data->debug)
                dprintf(2,">>> first_pipe\n");
                dup2(elem->pfd[1], 1);
            }
            else if(elem->prev && elem->next)
            {
                if(elem->data->debug)
                dprintf(2,">>> middle_pipe\n");
                dup2(elem->pfd[1], 1);
                dup2(elem->prev->pfd[0], 0);
            }
        }
        if(!elem->is_builtin)
        {
            if(elem->data->debug)
                dprintf(2,">>> fork\n");
            elem->pid = fork();
            if (elem->pid == 0)
            {
                if (elem->data->debug)
                    dprintf(2, ">>> child\n");
                else if (!elem->is_builtin && (execve(elem->cmd[0], elem->cmd, elem->data->envp) < 0))
                    ft_putstr_fd(strerror(errno), 2);
            }
            else
                wait(0);
        }
        else {
            if(elem->data->debug)
                dprintf(2,">>> builtin\n");
            builtin_exec(elem);
        }
		close_fd(elem);
        if(elem->data->debug)
            dprintf(2,">>> end of %p\n", elem);
        if (elem->next)
        {
            elem = elem->next;
            if(elem->data->debug)
            dprintf(2,">>> moving to %p\n", elem);
        }
        else
        {
            if(elem->pfd[1] != -1)
            {
                if(elem->data->debug)
                dprintf(2,">>> closing current pipe\n");
                close(elem->pfd[1]);
                close(elem->pfd[0]);
            }
            break;
        }
    }
}

void init(t_data *data, char **env)
{
    data->envp = env;
    data->std_in = dup(0);
    data->std_out = dup(1);
    data->error = 0;
    data->elem_start = NULL;
    data->simple_redirect_input_fd = -1;
    data->simple_redirect_output_fd = -1;
    data->double_redirect_output_fd = -1;
    env_path_find(data);
}

int main(int ac, char **av, char **env)
{
    char *cmd[4];
    char *cmd2[3];
    char *cmd3[3];
    char *cmd4[3];
    char *cmd5[2];
    char *cmd6[2];
    char *cmd7[2];
    t_data *data = malloc(sizeof (t_data));

    init(data, env);
    if(ac == 2)
        data->debug = 1;
    data->elem_start = push_back(data->elem_start, data);
//    data->elem_start = push_back(data->elem_start, data);
//    data->elem_start->next = push_back(data->elem_start->next, data);
//    data->elem_start->next->next = push_back(data->elem_start->next->next, data);
//    data->elem_start->next->next->next = push_back(data->elem_start->next->next->next, data);
//    data->elem_start->next->next->next->next = push_back(data->elem_start->next->next->next->next, data);
//    data->elem_start->next->next->next->next->next = push_back(data->elem_start->next->next->next->next->next, data);

    data->elem_start->cmd = cmd;
    data->elem_start->cmd[0] = "cd";
    data->elem_start->cmd[1] = "";
    data->elem_start->cmd[2] = 0;
    data->elem_start->cmd[3] = 0;
    data->elem_start->type = CMD;

//    data->elem_start->next->cmd = cmd2;
//    data->elem_start->next->cmd[0] = "env";
//    data->elem_start->next->cmd[1] = 0;
//    data->elem_start->next->cmd[2] = 0;
//    data->elem_start->next->type = CMD;
//
//    data->elem_start->next->next->cmd = cmd3;
//    data->elem_start->next->next->cmd[0] = "grep";
//    data->elem_start->next->next->cmd[1] = "2";
//    data->elem_start->next->next->cmd[2] = 0;
//    data->elem_start->next->next->type = CMD;
//
//    data->elem_start->next->next->next->cmd = cmd4;
//    data->elem_start->next->next->next->cmd[0] = "./text/old.txt";
//	data->elem_start->next->next->next->cmd[1] = 0;
//	data->elem_start->next->next->next->cmd[2] = 0;
//    data->elem_start->next->next->next->type = SIMPLE_REDIRECT_INPUT;
//
//    data->elem_start->next->next->next->next->cmd = cmd5;
//	data->elem_start->next->next->next->next->cmd[0] = "./text/out.txt";
//	data->elem_start->next->next->next->next->cmd[1] = 0;
//	data->elem_start->next->next->next->next->type = DOUBLE_REDIRECT_OUTPUT;
//
//	data->elem_start->next->next->next->next->next->cmd = cmd6;
//	data->elem_start->next->next->next->next->next->cmd[0] = "./text/in.txt";
//	data->elem_start->next->next->next->next->next->cmd[1] = 0;
//	data->elem_start->next->next->next->next->next->type = SIMPLE_REDIRECT_INPUT;
//
//	data->elem_start->next->next->next->next->next->next->cmd = cmd7;
//	data->elem_start->next->next->next->next->next->next->cmd[0] = "./text/out.txt";
//	data->elem_start->next->next->next->next->next->next->cmd[1] = 0;
//	data->elem_start->next->next->next->next->next->next->type = DOUBLE_REDIRECT_OUTPUT;

	simple_redirects(data);
//	printf(">>> DO %d SO %d SI %d\n", data->double_redirect_output_fd,data->simple_redirect_output_fd,data->simple_redirect_input_fd);
    execution(data->elem_start);
//    print_elems(data->elem_start);
    return 0;
}
