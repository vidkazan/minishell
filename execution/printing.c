#include "../main.h"

void print_current_elem(t_elem *ptr)
{
	dprintf(2,">>> %p, type %d next %p\n", ptr, ptr->type, ptr->next);
}

void print_elems(t_elem *root_ptr)
{
	dprintf(2,">>> \nprintelems\n");
	t_elem *ptr= root_ptr;
    if(!ptr)
    {
    	dprintf(2, ">>> (null)\n");
		return;
    }
    while(ptr->prev)
    	ptr = ptr->prev;
    while(ptr)
    {
        print_current_elem(ptr);
        if(!ptr->next)
            break;
        ptr = ptr->next;
    }
}