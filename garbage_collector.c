//
// Created by janbe on 28.11.2018.
//

#include "garbage_collector.h"
#include <stdlib.h>
#include "sax.h"


void gc_add_garbage(Tgarbage_collector *gc, void *ptr)
{
    GC_elem *elem = malloc(sizeof(GC_elem));
    elem->ptr = ptr;
    elem->previous = gc->last_added;
    gc->last_added = elem;
}

void valar_morghulis(Tgarbage_collector *gc)
{
    if (gc == NULL)
    {
        return;
    }
    TS_stack_free(gc->others->sym_stack);
    TS_stack_free(gc->others->sym_bin);
    symtab_free(gc->others->ts_func);

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