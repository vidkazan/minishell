#ifndef MAIN_H
#define MAIN_H

# include <sys/wait.h>
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
    int error;
    t_elem *elem_start;
    int simple_redirect_input_fd;
    int simple_redirect_output_fd;
    int double_redirect_output_fd;
    int debug;
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
void    builtin_check(t_elem *elem);
void    builtin_echo(t_elem *elem);
void    builtin_cd(t_elem *elem);
void builtin_pwd(t_elem *elem);

#endif