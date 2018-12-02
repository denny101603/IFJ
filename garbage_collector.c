//
// Created by janbe on 28.11.2018.
//

#include "garbage_collector.h"
#include <stdlib.h>


void gc_add_garbage(Tgarbage_collector *gc, void *ptr)
{
    GC_elem *elem = malloc(sizeof(GC_elem));
    elem->ptr = ptr;
    elem->previous = gc->last_added;
    gc->last_added = elem;
}

void gc_dealloc_all(Tgarbage_collector *gc)
{
    if (gc == NULL)
    {
        return;
    }

    while(gc->last_added != NULL)
    {
        //nacteni jednoho prvku z gc
        GC_elem *temp = gc->last_added;
        gc->last_added = gc->last_added->previous;
        free(temp->ptr);
        free(temp);
    }

    free(gc);
    gc = NULL;
}