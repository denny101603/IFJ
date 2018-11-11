//
// Created by janbe on 08.11.2018.
//

#include "sax.h"
#include "err_codes.h"

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

int startSA()
{
    TSynCommon sa_vars; //struktura s promennymi pro komunikacemi mezi castmi prekladace
    Tarray arr; //pole pro skener
    if(arr_init(&arr) == ERR_INTERNAL)
        return ERR_INTERNAL;
    else
        sa_vars.arr = &arr;

    sa_vars.ts_fun = symtab_init(TS_SIZE);
    if(sa_vars.ts_fun == NULL) //chyba alokace
    {
        arr_free(sa_vars.arr);
        return ERR_INTERNAL;
    }
    //todo pridat ostatni veci do savars - buffer atd
    sa_vars.err_code = IN_PROGRESS;
    while(sa_vars.err_code == IN_PROGRESS) //dokud je co prekladat, prekladam
        progr(&sa_vars);
    //TODO vse dealokovat
    return sa_vars.err_code;
}

bool progr(TSynCommon *sa_vars)
{
    Ttoken *token = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(token->type == KEY_DEF)
    {
        buffer_push_bottom(sa_vars->buffer, token);
        return nt_deffunc(sa_vars);
    }
    else if(token->type == EOL_1 || token->type == EOF_STATE)
    {
        buffer_push_bottom(sa_vars->buffer, token);
        return nt_eolf(sa_vars);
    }
    else if(token->type == KEY_WHILE)
    {
        buffer_push_bottom(sa_vars->buffer, token);
        return nt_cycl(sa_vars);
    }
    else if(token->type == KEY_IF)
    {
        buffer_push_bottom(sa_vars->buffer, token);
        return nt_ifthenelse(sa_vars);
    }
    else if(token->type == ID_2) //prislo ID: hrozi expression, assignment, callfce
    {
       Ttoken *look_ahead = get_next_token(sa_vars->arr, sa_vars->buffer);
       if(look_ahead->type == OP_EQAL_2) //prislo "=" tedy jednoznacne assignment
       {
           buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
           buffer_push_bottom(sa_vars->buffer, token);
           return nt_assignment(sa_vars);
       }
       else if(symtab_find(sa_vars->ts_fun token->attribute) == NULL) //neni v TS funkci, a je vylouceno i assignment takze je to expression
       { //TODO hrozi tady ty pitomosti kolem zatim nedefinovane fce
           buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
           buffer_push_bottom(sa_vars->buffer, token);
           return nt_expression(sa_vars);
       }
       else //ostatni jsem vyloucil, takze zbyva moznost volani fce
       {
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, token);
            return nt_callfce(sa_vars);
       }
    }
    else if(token->type == ID_FCE) //muze byt jen volani fce
    {
        if(symtab_find(sa_vars->ts_fun token->attribute) == NULL) //pokud neni v TS funkci, tak je to chyba
        {
            buffer_push_bottom(sa_vars->buffer, token);
            return false; //TODO mozna to neni chyba
        }
        else
        {
            buffer_push_bottom(sa_vars->buffer, token);
            return nt_callfce(sa_vars);
        }
    }
}

