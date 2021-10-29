#ifndef MAIN_H
#define MAIN_H

# include <sys/wait.h>
# include <sys/signal.h>
#include <signal.h>
# include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include <sys/errno.h>
#include "libft/libft.h"
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <curses.h>

#define CMD 1
#define PIPE 2
#define SIMPLE_REDIRECT_INPUT 3
#define SIMPLE_REDIRECT_OUTPUT 4
#define DOUBLE_REDIRECT_INPUT 5
#define DOUBLE_REDIRECT_OUTPUT 6


typedef struct s_data t_data;

typedef struct s_elem
{
    int type;
    int is_builtin;
    int pfd[2];
    pid_t pid;
    char **cmd;
    char *cal;
    struct s_elem *next;
    struct s_elem *prev;
    t_data  *data;
}               t_elem;

typedef struct s_data
{
    char *path;
    int std_in;
    int std_out;
    char **envp;
    int exit_status;
    t_elem *elem_start;
    int simple_redirect_input_fd;
    int simple_redirect_output_fd;
    int double_redirect_output_fd;
    int debug;

    int q1;
    int q2;
    char *line;
}              t_data;

t_elem  *create_elem(t_data *data);
t_elem  *push_back(t_elem *ptr, t_data *data);
void print_current_elem(t_elem *ptr,int id);
void print_elems(t_elem *ptr);
void	env_path_find(t_data *data);
void	find_path(t_elem *elem);
void simple_redirects(t_data *data);
t_elem 	*delete_current_node(t_elem	*elem);
t_elem	*double_redirect_output(t_elem	*elem);
char	*search_strings_in_array(char **arr, char *search_word, int *index);
void	edit_env_keys(int env_index, char *new_value, t_data *data);
char    **ft_arrdup(char **env);
void	free_arr(char **str);

void    builtin_exit(t_elem *elem,int write_fd);
void    builtin_unset(t_elem *elem,int write_fd);
void    builtin_check(t_elem *elem);
void    builtin_echo(t_elem *elem,int write_fd);
void    builtin_cd(t_elem *elem,int write_fd);
void builtin_pwd(t_elem *elem,int write_fd);
void builtin_env(t_elem *elem,int write_fd);
void builtin_export(t_elem *elem,int write_fd);
int   builtin_fd_gen(t_elem *elem);

void	list_cleaner(t_elem *elem);
void	main_preparser(t_data *data, char *line, char **env);
void	data_reboot(t_data *data, char *message, int mode);


#endif