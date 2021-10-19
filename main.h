#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include "libft/libft.h"

#define ELEM_COUNT 5
typedef struct s_elem
{
    int type;
    char *cmd;
    char **args;
    struct s_elem *next;
    struct s_elem *prev;
}               t_elem;

typedef struct s_data
{
    char **envp;
    int error;
    t_elem *elem_start;
}              t_data;

#endif