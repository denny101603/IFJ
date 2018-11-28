//
// Created by janbe on 28.11.2018.
//

#ifndef IFJ2018_GARBAGE_COLLECTOR_H
#define IFJ2018_GARBAGE_COLLECTOR_H
//Funkce a struktury pro garbage collector
typedef struct gce{
    void *ptr;
    struct gce *previous;
} GC_elem;

typedef struct gc{
    GC_elem *last_added;
} Tgarbage_collector;

void gc_add_garbage(void *ptr, Tgarbage_collector *gc);
void valar_morghulis(Tgarbage_collector *gc);

#endif //IFJ2018_GARBAGE_COLLECTOR_H
