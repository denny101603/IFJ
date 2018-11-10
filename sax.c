//
// Created by janbe on 08.11.2018.
//

#include "sax.h"

bool buffer_init(TBuffer *buffer_stack)
{
    buffer_stack->top = NULL;
    buffer_stack->bottom = NULL;
    return true;
}

bool buffer_push(TBuffer *buffer, Ttoken *token) //pushnuti na top
{
    TBufferElem *temp = malloc(sizeof(TBufferElem));
    if (temp == NULL)
        return false;
    temp->data = token;
    temp->next = NULL;
    if(buffer->bottom == NULL) //zasobnik prazdny
    {
        temp->prev = NULL;
        buffer->bottom = temp;
        buffer->top = temp;
    }
    else
    {
        temp->prev = buffer->top;
        buffer->top->next = temp;
        buffer->top = temp;
    }
    return true;
}

Ttoken *buffer_popTop(TBuffer *buffer) //pro savo
{
    TBufferElem *temp = buffer->top;
    Ttoken *ret = temp->data;
    if(buffer->top != buffer->bottom) // neni jediny prvek
    {
        buffer->top = temp->prev;
        buffer->top->next = NULL;
    }
    else
    {
        buffer->top = NULL;
        buffer->bottom = NULL;
    }
    free(temp);
    return ret;
}

Ttoken *buffer_popBottom(TBuffer *buffer) //rpo sax
{
    TBufferElem *temp = buffer->bottom;
    Ttoken *ret = temp->data;
    if(buffer->top == buffer->bottom) //zasobnik ma jediny prvek
    {
        buffer->top = NULL;
        buffer->bottom = NULL;
    }
    else
    {
        buffer->bottom = temp->next;
        buffer->bottom->prev = NULL;
    }
    free(temp);
    return ret;
}

void delete_buffer(TBuffer *buffer)
{
    TBufferElem *temp = NULL;// = stack->top;
    while (buffer->top != NULL)
    {
        temp = buffer->top;
        buffer->top = buffer->top->prev;
        if(temp != NULL) free(temp);
    }
    buffer->bottom = NULL;
    return;
}

bool buffer_empty(TBuffer *buffer)
{
    return (buffer->top == NULL);
}

Ttoken get_next_token(Tarray *arr, TBuffer *buffer)
{
    Ttoken ret;
    if(!buffer_empty(buffer))
        ret = *buffer_popBottom(buffer);
    else
        ret = get_token(arr);
    return ret;
}