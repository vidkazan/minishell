//
// Created by fcody on 10/21/21.
//

#include "main.h"

void builtin_pwd(t_elem *elem)
{
    char *pwd = getcwd(NULL, 1000);
    write(1, pwd , ft_strlen(pwd));
    write(1, "\n", 1);
    free(pwd);
}

void builtin_cd(t_elem *elem)
{
    char *PWD
    if(!elem->cmd[1])
    {
        if(chdir("/Users/fcody"))
            write(1, "error: chdir\n", 13);
    }
    else
    {
        if(chdir(elem->cmd[1]))
            write(1, "error: chdir\n", 13);
    }
    builtin_pwd(elem);
}