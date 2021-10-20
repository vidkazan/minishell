#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include <sys/errno.h>
#include "libft/libft.h"

#define ELEM_COUNT 5
#define CMD 1
#define PIPE 2
#define REDIRECT_INPUT 3
#define REDIRECT_OUTPUT 4
#define DOUBLE_REDIRECT_INPUT 5
#define DOUBLE_REDIRECT_OUTPUT 6

typedef struct s_data t_data;

typedef struct s_elem
{
    int type;
    int pfd[2];
    pid_t pid;
    char **cmd;
    struct s_elem *next;
    struct s_elem *prev;
    t_data  *data;
}               t_elem;

typedef struct s_data
{
    int std_in;
    int std_out;
    char **envp;
    int error;
    t_elem *elem_start;
}              t_data;

t_elem  *create_elem(t_data *data);
t_elem  *push_back(t_elem *ptr, t_data *data);
void print_current_elem(t_elem *ptr,int id);
void print_elems(t_elem *ptr);

#endif