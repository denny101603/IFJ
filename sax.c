//
// Created by janbe on 08.11.2018.
//

#include "sax.h"
#include "savo.h"
#include "err_codes.h"
#include "savo.h"

bool buffer_init(TBuffer *buffer_stack)
{
    buffer_stack->top = NULL;
    buffer_stack->bottom = NULL;
    return true;
}

bool buffer_push_bottom(TBuffer *buffer, Ttoken *token) //push na bottom == na dno. Pouzivat opatrne
{
    TBufferElem *temp = malloc(sizeof(TBufferElem));
    if (temp == NULL)
        return false;
    temp->data = token;
    temp->prev = NULL;
    if(buffer->bottom == NULL) //zasobnik prazdny
    {
        temp->next = NULL;
        buffer->bottom = temp;
        buffer->top = temp;
    }
    else
    {
        temp->next = buffer->bottom;
        buffer->bottom->prev = temp;
        buffer->bottom = temp;
    }
    return true;
}

bool buffer_push_top(TBuffer *buffer, Ttoken *token) //pushnuti na top
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
}

bool buffer_empty(TBuffer *buffer)
{
    return (buffer->top == NULL);
}

//Funkce pro praci se stackem tabulek symbolů

void TS_stack_init(TSymtables_stack *stack)
{
    stack->top = NULL;
    stack->bottom = NULL;
}

bool TS_push(TSymtables_stack *stack, Tsymbol_table *table)
{
    TLTElem *temp = malloc(sizeof(TLTElem));
    if(temp == NULL)
        return false;
    temp->data = table;
    temp->prev = stack->top;
    if(stack->bottom == NULL) //zasobnik je prazdny
        stack->bottom = temp;
    stack->top = temp;
    return true;
}

Tsymbol_table *TS_pop(TSymtables_stack *stack)
{
    TLTElem *temp;
    Tsymbol_table *ret;
    temp = stack->top;
    ret = temp->data;
    if(stack->top == stack->bottom) // jeden prvek v zasobobniku
        stack->bottom = NULL;
    stack->top = stack->top->prev;
    free(temp);
    return ret;
}


Ttoken *get_next_token(Tarray *arr, TBuffer *buffer)
{
    Ttoken *ret = NULL;
    if(!buffer_empty(buffer))
        ret = buffer_popBottom(buffer);
    else
        ret = get_token(arr);
    return ret;
}

int startSA()
{
    TSynCommon sa_vars; //struktura s promennymi pro komunikacemi mezi castmi prekladace //todo asi lepsi dynalokovat
    Tarray *arr = (Tarray *) malloc(sizeof(Tarray)); //struktura pole pro skener
    if(arr_init(arr) == ERR_INTERNAL) //neuspesna alokace
    {
        free(arr);
        return ERR_INTERNAL;
    }
    else
        sa_vars.arr = arr;

    sa_vars.ts_fun = symtab_init(TS_SIZE);
    if(sa_vars.ts_fun == NULL) //chyba alokace
    {
        arr_free(sa_vars.arr);
        free(arr);
        return ERR_INTERNAL;
    }

    //todo denny pridat ostatni veci do sa_vars - buffer atd
    TBuffer *buffer = (TBuffer *) malloc(sizeof(TBuffer));
    if(buffer == NULL) //neuspesna alokace
    {
        arr_free(sa_vars.arr);
        free(arr);
        symtab_free(sa_vars.ts_fun);
        return ERR_INTERNAL;
    }
    else
    {
        buffer_init(buffer);
        sa_vars.buffer = buffer;
    }

    TSymtables_stack *local_tables = (TSymtables_stack *) malloc(sizeof(TSymtables_stack));
    Tsymbol_table *symtab_local = symtab_init(TS_SIZE);
    if(local_tables == NULL || symtab_local == NULL) //neuspesna alokace
    {
        arr_free(sa_vars.arr);
        free(arr);
        symtab_free(sa_vars.ts_fun);
        free(local_tables);
        free(symtab_local);
        return ERR_INTERNAL;
    }
    else
    {
        TS_stack_init(local_tables);
        sa_vars.local_tables = local_tables;
        TS_push(sa_vars.local_tables, symtab_local);
    }

    sa_vars.err_code = IN_PROGRESS;
    while(sa_vars.err_code == IN_PROGRESS) //dokud je co prekladat, prekladam
        if(!progr(&sa_vars)) //todo denny uupravit asi
            break;
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
       else if(symtab_find(sa_vars->ts_fun, token->attribute) == NULL) //neni v TS funkci, a je vylouceno i assignment takze je to expression
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
        if(symtab_find(sa_vars->ts_fun, token->attribute) == NULL) //pokud neni v TS funkci, tak je to chyba
        {
            buffer_push_bottom(sa_vars->buffer, token);
            return false; //TODO denny mozna to neni chyba
        }
        else
        {
            buffer_push_bottom(sa_vars->buffer, token);
            return nt_callfce(sa_vars);
        }
    }
    else
        return false;
}

bool nt_deffunc(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_DEF)
        return false;
    Ttoken *t2 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t2->type == ID_FCE || t2->type == ID_2)
    {
        if(symtab_find(sa_vars->ts_fun, t2->attribute) == NULL && symtab_find(sa_vars->local_tables->bottom->data, t2->attribute) == NULL) //pokud jeste takove ID neexistuje
        {
            symtab_edit_add(sa_vars->ts_fun, t2->attribute, true, t2->type, 0); //pohlidat spravny pocet parametru
            TS_push(sa_vars->local_tables, symtab_init(TS_SIZE)); //vytvorim novou lokalni TS pro telo fce

            Ttoken *t3 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if (t3->type != LEFT_BRACKET)
                return false;
            if (!nt_params(sa_vars))
                return false;
            Ttoken *t4 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if (t4->type != RIGHT_BRACKET)
                return false;
            Ttoken *t5 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if (t5->type != EOL_1)
                return false;
            if (!nt_bodyfce(sa_vars))
                return false;
            Ttoken *t6 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if (t6->type != KEY_END)
                return false;

            if (!nt_eolf(sa_vars))
                return false;

            return true; //uz jsem na konci, vse probehlo OK
        }
        else
            return false;
    }
    else
        return false;
}

bool nt_bodyfce(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == EOL_1) //RULE 8
    {
        if(nt_bodyfce(sa_vars))
        {
            return true;
        }
    }
    else if(t1->type == KEY_DEF) //RULE 6
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_deffunc(sa_vars))
        {
            return false;
        }
        return nt_bodyfce(sa_vars);
    }
    else if(t1->type == KEY_IF) //RULE10
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_ifthenelse(sa_vars))
        {
            return false;
        }
        return nt_bodyfce(sa_vars);
    }
    else if(t1->type == KEY_WHILE) //RULE9
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_cycl(sa_vars))
        {
            return false;
        }
        return nt_bodyfce(sa_vars);
    }
    else if(((t1->type == ID_2) || (t1->type == ID_FCE)) && (symtab_find(sa_vars->ts_fun,t1->attribute) != NULL)) //RULE 13
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_callfce(sa_vars))
        {
            return false;
        }
        return nt_bodyfce(sa_vars);
    }
    else if((t1->type == ID_2) && (symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)) //RULE11
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_assignment(sa_vars))
        {
            return false;
        }
        return nt_bodyfce(sa_vars);
    }
    else if(t1->type == KEY_END)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(savo(sa_vars))
        {
            return nt_bodyfce(sa_vars);
        }
        else
            return false;
    }
}

bool nt_ifthenelse(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_IF) //IF
    {
        token_free(t1);
        return false;
    }
    if(!savo(sa_vars)) //EXPR
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_THEN) //THEN
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != EOL_1) //EOL
    {
        token_free(t1);
        return false;
    }
    if(!nt_bodywhif(sa_vars))           //IFBODY
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_ELSE) //ELSE
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != EOL_1) //EOL
    {
        token_free(t1);
        return false;
    }
    if(!nt_bodywhif(sa_vars))           //ELSEBODY
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_END)             //END
    {
        token_free(t1);
        return false;
    }
    if(nt_eolf(sa_vars))                //EOLFmrdat
    {
        token_free(t1);
        return true;
    }
    else
    {
        token_free(t1);
        return false;
    }
}

bool nt_cycl(TSynCommon *sa_vars)       //cycl -> WHILE EXPR  DO EOL bodywhif END eolf
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_WHILE)           //WHILE
    {
        token_free(t1);
        return false;
    }
    if(!savo(sa_vars))                       //EXPR
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_DO)              //DO
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != EOL_1)               //EOL
    {
        token_free(t1);
        return false;
    }
    if(!nt_bodywhif(sa_vars))           //bodywhif
    {
        token_free(t1);
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type != KEY_END)             //END
    {
        token_free(t1);
        return false;
    }
    if(nt_eolf(sa_vars))                //eolf
    {
        token_free(t1);
        return true;
    }
    else
    {
        token_free(t1);
        return false;
    }

}

bool nt_params(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == RIGHT_BRACKET)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
        if(t1->type == ID_2)
        {
            if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)
            {
                token_free(t1);
                return false;
                //je uz definovana
            }
            else
            {
                if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
                {
                    token_free(t1);
                    return false;
                    //je to id_fce
                }
                else
                {
                    symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, 0, t1->type);
                    if(nt_nextparams(sa_vars))
                    {
                        token_free(t1);
                        return true;
                    }
                    else
                    {
                        token_free(t1);
                        return false;
                    }
                    //neni definovana ani ve func --> add_id
                }
            }
        }
    }
}

bool nt_nextparams(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == RIGHT_BRACKET || t1->type == OP_COMMA)
    {
        if(t1->type == RIGHT_BRACKET)
        {
            buffer_push_bottom(sa_vars->buffer,t1);
            return true;
        }
        else
        {
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(t1->type == ID_2)
            {
                if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)
                {
                    token_free(t1);
                    return false;
                    //je uz definovana
                }
                else
                {
                    if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
                    {
                        token_free(t1);
                        return false;
                        //je to id_fce
                    }
                    else
                    {
                        symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, 0, t1->type);
                        if(nt_nextparams(sa_vars))
                        {
                            token_free(t1);
                            return true;
                        }
                        else
                        {
                            token_free(t1);
                            return false;
                        }
                        //neni definovana ani ve func --> add_id
                    }
                }
            }
        }
    }
    else
    {
        token_free(t1);
        return false;
    }
}

bool nt_assignment(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == ID_2)
    {
        if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL && symtab_find(sa_vars->ts_fun, t1->attribute) == NULL)
        {
            //je uz definovana
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(t1->type != OP_EQAL_2)       //=
            {
                token_free(t1);
                return false;
            }
            if(nt_right(sa_vars))           //right
            {
                token_free(t1);
                return true;
            }
            else
            {
                token_free(t1);
                return false;
            }
        }
        else
        {
            token_free(t1);
            return false;
            //neni definovana nebo je ve func
        }
    }
    else
    {
        token_free(t1); //test
        return false;
    }
}

bool nt_right(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
    {
        //je to id_fce
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        //cekam EXPR
        buffer_push_bottom(sa_vars->buffer, t1);
        if(savo(sa_vars))
        {
            if(nt_eolf(sa_vars))                //eolf
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            token_free(t1);
            return false;
        }
    }
}

bool nt_eolf(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if((t1->type == EOL_1)) //EOF NEBO EOL
    {
        token_free(t1);
        return true;
    }
    else if(t1->type == EOF_STATE)
    {
        sa_vars->err_code = 101; //todo predelat
        return true;
    }
    else
    {
        token_free(t1);
        return false;
    }
}

bool nt_expression(TSynCommon *sa_vars)
{
    if(!savo(sa_vars))
    {
        return false;
    }
    return nt_eolf(sa_vars);
}

bool nt_nextargs(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == OP_COMMA) //RULE33
    {
        if(!savo(sa_vars))
        {
            return false;
        }
        return nt_nextargs(sa_vars);
    }
    else if((t1->type == RIGHT_BRACKET) || (t1->type == EOL_1) || (t1->type == EOF_STATE)) //RULE 32
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    } else {
        buffer_push_bottom(sa_vars->buffer, t1);
        return false;
    }
}

bool nt_args(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if((t1->type == RIGHT_BRACKET) || (t1->type == EOL_1) || (t1->type == EOF_STATE))
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    buffer_push_bottom(sa_vars->buffer, t1);
    if(!savo(sa_vars))
    {
        return false;
    }
    return nt_nextargs(sa_vars);
}

bool nt_right(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
    {
        //je to id_fce
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        //cekam EXPR
        buffer_push_bottom(sa_vars->buffer, t1);
        if(savo(sa_vars))
        {
            if(nt_eolf(sa_vars))                //eolf
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            token_free(t1);
            return false;
        }
    }
}

bool nt_assignment(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == ID_2)
    {
        if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //neni to funkce
        {
            if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) == NULL) //jeste neni definovana
                symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, 1, t1->type, 0); //pridani promenne do lokalni TS

            //uz je definovana
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(t1->type != OP_EQAL_2)       //=
            {
                token_free(t1);
                return false;
            }
            if(nt_right(sa_vars))           //right
            {
                token_free(t1);
                return true;
            }
            else
            {
                token_free(t1);
                return false;
            }
        }
        else //neni to validni leva strana prirazeni
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return false;
        }
    }
    else //neni to ID_2
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return false;
    }
}

bool nt_nextparams(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == RIGHT_BRACKET || t1->type == OP_COMMA)
    {
        if(t1->type == RIGHT_BRACKET)
        {
            buffer_push_bottom(sa_vars->buffer,t1);
            return true;
        }
        else
        {
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(t1->type == ID_2)
            {
                if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)
                {
                    token_free(t1);
                    return false;
                    //je uz definovana
                }
                else
                {
                    if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
                    {
                        token_free(t1);
                        return false;
                        //je to id_fce
                    }
                    else
                    {
                        symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, 0, t1->type);
                        if(nt_nextparams(sa_vars))
                        {
                            token_free(t1);
                            return true;
                        }
                        else
                        {
                            token_free(t1);
                            return false;
                        }
                        //neni definovana ani ve func --> add_id
                    }
                }
            }
        }
    }
    else
    {
        token_free(t1);
        return false;
    }
}

bool nt_params(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == RIGHT_BRACKET)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
        if(t1->type == ID_2)
        {
            if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)
            {
                token_free(t1);
                return false;
                //je uz definovana
            }
            else
            {
                if(symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
                {
                    token_free(t1);
                    return false;
                    //je to id_fce
                }
                else
                {
                    symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, 0, t1->type);
                    if(nt_nextparams(sa_vars))
                    {
                        token_free(t1);
                        return true;
                    }
                    else
                    {
                        token_free(t1);
                        return false;
                    }
                    //neni definovana ani ve func --> add_id
                }
            }
        }
    }
}

bool nt_callfce(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!(t1->type == ID_2 || t1->type == ID_FCE)) //neni IDFCE
    {
        token_free(t1);
        return false;
    }
    if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //neni v TS funkci
    {
        token_free(t1);
        return false;
    }
    //todo denny poresit volitelne zavorky

    if(!nt_args(sa_vars))
        return false;

    if(!nt_eolf(sa_vars))
        return false;

    return true;
}

bool nt_bodywhif(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(t1->type == EOL_1)
    {
        if(nt_bodyfce(sa_vars))
        {
            return true;
        }
    }
    else if(t1->type == KEY_IF)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_ifthenelse(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if(t1->type == KEY_WHILE)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_cycl(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if(((t1->type == ID_2) || (t1->type == ID_FCE)) && (symtab_find(sa_vars->ts_fun,t1->attribute) != NULL)) //RULE 13
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_callfce(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if((t1->type == ID_2) && (symtab_find(sa_vars->local_tables->top->data, t1->attribute) != NULL)) //RULE11
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_assignment(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if(t1->type == KEY_END)
    {
        return true;
    } else {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!savo(sa_vars))
        {
            return nt_bodywhif(sa_vars);
        }
    }
}