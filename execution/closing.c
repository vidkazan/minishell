#include "../main.h"

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

void closing(t_data *data)
{
    free_arr(data->envp);
}