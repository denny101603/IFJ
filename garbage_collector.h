//
// Created by janbe on 28.11.2018.
//

#ifndef IFJ2018_GARBAGE_COLLECTOR_H
#define IFJ2018_GARBAGE_COLLECTOR_H

#include "sax.h"

//Funkce a struktury pro garbage collector
typedef struct gce{
    void *ptr;
    struct gce *previous;
} GC_elem;

typedef struct other{
    TSymtables_stack *sym_stack;
    TSymtables_stack *sym_bin;
    Tsymbol_table *ts_func;
} Tothers;

typedef struct gc{
    GC_elem *last_added;
    Tothers *others;
} Tgarbage_collector;


void gc_add_garbage(Tgarbage_collector *gc, void *ptr);

void valar_morghulis(Tgarbage_collector *gc);


#endif //IFJ2018_GARBAGE_COLLECTOR_H
