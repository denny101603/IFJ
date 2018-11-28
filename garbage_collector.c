//
// Created by janbe on 28.11.2018.
//

#include "garbage_collector.h"
#include <stdlib.h>

//Funkce a struktury pro garbage collector
typedef struct gce{
    void *ptr;
    struct gce *previous;
} GC_elem;

typedef struct gc{
    GC_elem *last_added;
} garbage_collector;

void gc_add_garbage(void *ptr, garbage_collector *gc)
{
    GC_elem *elem = malloc(sizeof(GC_elem));
    elem->ptr = ptr;
    elem->previous = gc->last_added;
    gc->last_added = elem;
}

void valar_morghulis(garbage_collector *gc)
{
    while(gc->last_added != NULL)
    {
        //nacteni jednoho prvku z gc
        GC_elem *temp = gc->last_added;
        gc->last_added = gc->last_added->previous;

        free(temp->ptr);
        free(temp);
    }
}