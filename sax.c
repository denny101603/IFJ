//
// Created by janbe on 08.11.2018.
//

#include "sax.h"
#include "savo.h"
#include "seman.h"
#include "err_codes.h"
#include <string.h>

void buffer_init(TBuffer *buffer_stack)
{
    buffer_stack->top = NULL;
    buffer_stack->bottom = NULL;
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
    if(buffer == NULL || buffer->top == NULL) //err handle: bud predan ukazatel na neexistujici buffer, nebo na prazdny buffer
        return  NULL;
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
    if(buffer == NULL)
        return NULL;
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
        if(temp != NULL)
        {
            if(temp->data != NULL) token_free(temp->data);
            free(temp);
        }
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
    if(stack == NULL)
        return NULL;
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
    TSynCommon *sa_vars = alloc_sa();
    if(sa_vars == NULL)
        return ERR_INTERNAL;

    sa_vars->err_code = IN_PROGRESS;
    while(sa_vars->err_code == IN_PROGRESS) //dokud je co prekladat, prekladam
    {
        if (!progr(sa_vars)) //todo denny upravit asi
        {
            if (sa_vars->err_code == IN_PROGRESS) //pokud neni doplnen jiny err_code, nastala syntakticka chyba
            {
                sa_vars->err_code = ERR_SYN;
                fprintf(stderr, MESSAGE_SYN);
            }
            break;
        }
    }
    int ret = sa_vars->err_code;
    dealloc_sa(sa_vars);
    return ret;
}

bool progr(TSynCommon *sa_vars)
{
    Ttoken *token = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(token, sa_vars))
        return false;

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
        if(!err_check(token, sa_vars))
        {
            token_free(token);
            return false;
        }
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
    else //zbyva snad uz jen vyraz
    {
        buffer_push_bottom(sa_vars->buffer, token);
        return nt_expression(sa_vars);
    }
}

bool nt_deffunc(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_DEF)
        return false;
    token_free(t1);
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;

    if(t1->type == ID_FCE || t1->type == ID_2)
    {   //todo denny nemel bych vzdy prochazet vsechny i lokalni TS?
        if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL && symtab_find(sa_vars->local_tables->bottom->data, t1->attribute) == NULL) //pokud jeste takove ID neexistuje
        {
            symtab_edit_add(sa_vars->ts_fun, t1->attribute, true, NOBODY_CARES);
            TS_push(sa_vars->local_tables, symtab_init(TS_SIZE)); //vytvorim novou lokalni TS pro telo fce
            Ttoken *t2 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
            {
                token_free(t1);
                return false;
            }
            if (t2->type != LEFT_BRACKET)
                return false;
            token_free(t2);

            //todo seman: DEFFUNC(op1: t1->attribute)

            if (!nt_params(sa_vars))
                return false;

            //uz znam pocet parametru fce
            symtab_edit_add(sa_vars->ts_fun, t1->attribute, true, (long int) symtab_get_size(sa_vars->local_tables->top->data));
            free(t1);

            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
                return false;

            if (t1->type != RIGHT_BRACKET)
                return false;
            token_free(t1);

            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
                return false;

            if (t1->type != EOL_1)
                return false;
            token_free(t1);

            if (!nt_bodyfce(sa_vars))
                return false;

            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
                return false;

            if (t1->type != KEY_END)
                return false;
            token_free(t1);

            //todo denny - muzu ji zahodit? nemel bych je nekde spis skladovat kvuli kolizi ID lokalni promenne a fce..
            symtab_free(TS_pop(sa_vars->local_tables));

            if (!nt_eolf(sa_vars))
                return false;

            return true; //uz jsem na konci, vse probehlo OK
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
    }
}

bool nt_cycl(TSynCommon *sa_vars)       //cycl -> WHILE EXPR  DO EOL bodywhif END eolf
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_WHILE)           //WHILE
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    sa_vars->boolean = true; //muze to byt typ boolean
    if(!savo(sa_vars))                       //EXPR
    {
        return false;
    }
    sa_vars->boolean = false; //vracim do vychoziho stavu

    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_DO)              //DO
    {
        token_free(t1);
        return false;
    }
    token_free(t1);
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != EOL_1)               //EOL
    {
        token_free(t1);
        return false;
    }
    token_free(t1);
    if(!nt_bodywhif(sa_vars))           //bodywhif
    {
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
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
        token_free(t1);
        sa_vars->err_code = SUCCESS;
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

bool nt_ifthenelse(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_IF) //IF
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    sa_vars->boolean = true; //muze byt typu bool
    if(!savo(sa_vars)) //EXPR
    {
        return false;
    }
    sa_vars->boolean = false; //vracim do vychoziho stavu

    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_THEN) //THEN
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != EOL_1) //EOL
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    if(!nt_bodywhif(sa_vars))           //IFBODY
    {
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_ELSE) //ELSE
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != EOL_1) //EOL
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    if(!nt_bodywhif(sa_vars))           //ELSEBODY
    {
        return false;
    }
    t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type != KEY_END)             //END
    {
        token_free(t1);
        return false;
    }
    token_free(t1);

    if(nt_eolf(sa_vars))                //EOLF
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool nt_args(TSynCommon *sa_vars, long *num_of_args)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if((t1->type == RIGHT_BRACKET) || (t1->type == EOL_1) || (t1->type == EOF_STATE))
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    buffer_push_bottom(sa_vars->buffer, t1);

    //todo seman: defvar(op1: vygeneruju nejakou docasnou promennou_#42)

    if(!savo(sa_vars)) //todo seman: reknu savu aby mi to dal do tte docasne promenne_#42
    {
        return false;
    }
    (*num_of_args)++; //pribyl argument
    if(nt_nextargs(sa_vars, num_of_args))
    {
        //todo seman: PUSH(op1: ta docasna promenna_#42)
        return true;
    }
    else
        return false;
}

bool nt_nextargs(TSynCommon *sa_vars, long *num_of_args)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == OP_COMMA) //RULE33
    {
        //todo seman: defvar(op1: vygeneruju nejakou docasnou promennou_#41)

        if(!savo(sa_vars)) //todo seman: reknu savu aby mi to dal do tte docasne promenne_#41
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return false;
        }
        (*num_of_args)++; //pribyl argument
        if(nt_nextargs(sa_vars, num_of_args))
        {
            //todo seman: PUSH(op1: ta docasna promenna_#41)

            token_free(t1); //povedlo se, muzu uvolnit
            return true;
        }
        else
        {
            buffer_push_bottom(sa_vars->buffer, t1); //nepovedlo se, musim vratit pro pripadny dalsi pokus
            return false;
        }
    }
    else if((t1->type == RIGHT_BRACKET) || (t1->type == EOL_1) || (t1->type == EOF_STATE)) //RULE 32
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return false;
    }
}

bool nt_right(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == ID_FCE || t1->type == ID_2)
    {
        if (symtab_find(sa_vars->ts_fun, t1->attribute) != NULL)
        {
            //je to id_fce
            buffer_push_bottom(sa_vars->buffer, t1);
            if(nt_callfce(sa_vars)) //todo seman: predam destinaci fci callfce, ta to tam ulozi
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    //cekam EXPR
    buffer_push_bottom(sa_vars->buffer, t1);
    if(savo(sa_vars)) //todo seman: predam destinaci kam ma vyraz ulozit
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

bool nt_assignment(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == ID_2)
    {
        if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //neni to funkce
        {
            if(symtab_find(sa_vars->local_tables->top->data, t1->attribute) == NULL) //jeste neni definovana
            {
                symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, 1, NOBODY_CARES); //pridani promenne do lokalni TS
                //todo seman: defvar(op1: t1->attribute)
                free(t1);
            }
            else
                token_free(t1);

            //uz je definovana
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
                return false;
            if(t1->type != OP_EQAL_2)       //=
            {
                token_free(t1);
                return false;
            }
            token_free(t1);

            if(nt_right(sa_vars))           //right
            {
                return true;
            }
            else
            {
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
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == RIGHT_BRACKET || t1->type == OP_COMMA)
    {
        if(t1->type == RIGHT_BRACKET)
        {
            buffer_push_bottom(sa_vars->buffer,t1);
            return true;
        }
        else
        {
            token_free(t1);
            t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
            if(!err_check(t1, sa_vars))
                return false;
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
                        symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, NOBODY_CARES);
                        //todo seman: loadparam(dest: t1->attribute)

                        free(t1);
                        if(nt_nextparams(sa_vars))
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                        //neni definovana ani ve func --> add_id
                    }
                }
            }
            else //neni ID_2
            {
                token_free(t1);
                return false;
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
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == RIGHT_BRACKET)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    }
    else
    {
        //t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
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
                    symtab_edit_add(sa_vars->local_tables->top->data, t1->attribute, true, NOBODY_CARES);

                    //todo seman: loadparam(dest. t1->attribute)

                    free(t1);

                    if(nt_nextparams(sa_vars))
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                    //neni definovana ani ve func --> add_id
                }
            }
        }
        else //neni ID_2
        {
            token_free(t1);
            return false;
        }
    }
}

bool nt_callfce(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;

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

    long num_of_args = 0; //pocitadlo argumentu pro overeni s TS
    if(!nt_args(sa_vars, &num_of_args)) //nepovedlo se, zkusim tedy jestli nebyla zavorka, pokud ano, zkusim to znovu
    {
        Ttoken *t2 = get_next_token(sa_vars->arr, sa_vars->buffer);
        //if(err_check())
        token_free(t1);
        return false;
    }

    if(!check_num_of_params(sa_vars->ts_fun, t1, num_of_args)) //nesedi pocet parametru s definici
    {
        token_free(t1);
        sa_vars->err_code = ERR_SEM_PARAM;
        fprintf(stderr, MESSAGE_SEM_PARAM);
        return false;
    }

    //todo seman: call(dest: to bychom tu meli vedet, op1: t1->attribute (LABEL))

    token_free(t1);
    if(!nt_eolf(sa_vars))
        return false;

    return true;
}

bool nt_bodyfce(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == EOL_1) //RULE 8
    {
        if(nt_bodyfce(sa_vars))
        {
            token_free(t1);
            return true;
        }
        token_free(t1);
        return false;
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
    else if(t1->type == ID_2) //prislo ID: hrozi expression, assignment, callfce
    {
        Ttoken *look_ahead = get_next_token(sa_vars->arr, sa_vars->buffer);
        if(!err_check(look_ahead, sa_vars))
            return false;
        if(look_ahead->type == OP_EQAL_2) //prislo "=" tedy jednoznacne assignment
        {
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(!nt_assignment(sa_vars))
            {
                return false;
            }
            return nt_bodyfce(sa_vars);
        }
        else if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //neni v TS funkci, a je vylouceno i assignment takze je to expression
        { //TODO hrozi tady ty pitomosti kolem zatim nedefinovane fce
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(nt_expression(sa_vars))
            {
                return nt_bodyfce(sa_vars);
            }
            return false;
        }
        else //ostatni jsem vyloucil, takze zbyva moznost volani fce
        {
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(nt_callfce(sa_vars))
            {
                return nt_bodyfce(sa_vars);
            }
            return false;
        }
    }
    else if(t1->type == ID_FCE) //muze byt jen volani fce
    {
        if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //pokud neni v TS funkci, tak je to chyba
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return false; //TODO denny mozna to neni chyba
        }
        else
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return nt_callfce(sa_vars);
        }
    }
    else if(t1->type == KEY_END)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    } else{
        return false;
    }
}

bool nt_bodywhif(TSynCommon *sa_vars)
{
    Ttoken *t1 = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(!err_check(t1, sa_vars))
        return false;
    if(t1->type == EOL_1) //RULE 8
    {
        if(nt_bodywhif(sa_vars))
        {
            token_free(t1);
            return true;
        }
        token_free(t1);
        return false;
    }
    else if(t1->type == KEY_IF) //RULE10
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_ifthenelse(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if(t1->type == KEY_WHILE) //RULE9
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        if(!nt_cycl(sa_vars))
        {
            return false;
        }
        return nt_bodywhif(sa_vars);
    }
    else if(t1->type == ID_2) //prislo ID: hrozi expression, assignment, callfce
    {
        Ttoken *look_ahead = get_next_token(sa_vars->arr, sa_vars->buffer);
        if(!err_check(look_ahead, sa_vars))
            return false;
        if(look_ahead->type == OP_EQAL_2) //prislo "=" tedy jednoznacne assignment
        {
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(!nt_assignment(sa_vars))
            {
                return false;
            }
            return nt_bodywhif(sa_vars);
        }
        else if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //neni v TS funkci, a je vylouceno i assignment takze je to expression
        { //TODO hrozi tady ty pitomosti kolem zatim nedefinovane fce
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(nt_expression(sa_vars))
            {
                return nt_bodywhif(sa_vars);
            }
            return false;
        }
        else //ostatni jsem vyloucil, takze zbyva moznost volani fce
        {
            buffer_push_bottom(sa_vars->buffer, look_ahead); //vraceni tokenu ve spravnem poradi
            buffer_push_bottom(sa_vars->buffer, t1);
            if(nt_callfce(sa_vars))
            {
                return nt_bodywhif(sa_vars);
            }
            return false;
        }
    }
    else if(t1->type == ID_FCE) //muze byt jen volani fce
    {
        if(symtab_find(sa_vars->ts_fun, t1->attribute) == NULL) //pokud neni v TS funkci, tak je to chyba
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return false; //TODO denny mozna to neni chyba
        }
        else
        {
            buffer_push_bottom(sa_vars->buffer, t1);
            return nt_callfce(sa_vars);
        }
    }
    else if(t1->type == KEY_END  || t1->type == KEY_ELSE)
    {
        buffer_push_bottom(sa_vars->buffer, t1);
        return true;
    } else{
        return false;
    }
}


bool init_ts_fun(TSynCommon *sa_vars)
{
    sa_vars->ts_fun = symtab_init(TS_SIZE);
    if(sa_vars->ts_fun == NULL) //chyba alokace
        return false;

    char *inputs = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *inputi = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *inputf = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *print = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *lenght = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *substr = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *ord = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)
    char *chr = (char *) malloc(sizeof(char) * 7); //7 je potrebna delka pro nejdelsi nazev integrovane fce (6znaku + ukoncovaci)

    if(inputs == NULL || inputi == NULL || inputf == NULL || print == NULL
        || lenght == NULL || substr == NULL || ord == NULL || chr == NULL) //nepovedena alokace
    {
        free(inputs);
        free(inputi);
        free(inputf);
        free(print);
        free(lenght);
        free(substr);
        free(ord);
        free(chr);
        symtab_free(sa_vars->ts_fun);
        return false;
    }

    strcpy(inputs, "inputs");
    strcpy(inputi, "inputi");
    strcpy(inputf, "inputf");
    strcpy(print, "print");
    strcpy(lenght, "lenght");
    strcpy(substr, "substr");
    strcpy(ord, "ord");
    strcpy(chr, "chr");

    Tsymbol_table_item *a = symtab_edit_add(sa_vars->ts_fun, inputs, true, 0);
    Tsymbol_table_item *b = symtab_edit_add(sa_vars->ts_fun, inputi, true, 0);
    Tsymbol_table_item *c = symtab_edit_add(sa_vars->ts_fun, inputf, true, 0);
    Tsymbol_table_item *d = symtab_edit_add(sa_vars->ts_fun, print, true, ZERO_TO_INF);
    Tsymbol_table_item *e = symtab_edit_add(sa_vars->ts_fun, lenght, true, 1);
    Tsymbol_table_item *f = symtab_edit_add(sa_vars->ts_fun, substr, true, 3);
    Tsymbol_table_item *g = symtab_edit_add(sa_vars->ts_fun, ord, true, 2);
    Tsymbol_table_item *h = symtab_edit_add(sa_vars->ts_fun, chr, true, 1);

    if(a == NULL || b == NULL || c == NULL || d == NULL || e == NULL || f == NULL || g == NULL || h == NULL) //neuspesna alokace
    {
        free(inputs);
        free(inputi);
        free(inputf);
        free(print);
        free(lenght);
        free(substr);
        free(ord);
        free(chr);
        free(a);
        free(b);
        free(c);
        free(e);
        free(d);
        free(f);
        free(g);
        free(h);
        free(sa_vars->ts_fun);
        return false;
    }

    return true;
}

bool err_check(Ttoken *t, TSynCommon *sa_vars)
{
    if(t == NULL) //chyba alokace
        sa_vars->err_code = ERR_INTERNAL;
    else if(t->type == LEX_ERROR) //lexikalni chyba
        sa_vars->err_code = ERR_LEX;
    else if(t->type == ERR_INTERNAL) //chyba alokace
        sa_vars->err_code = ERR_INTERNAL;
    else
        return true; //neni problem

    token_free(t);
    return false; //chyba
}

TSynCommon *alloc_sa()
{
    TSynCommon *sa_vars = (TSynCommon *) malloc(sizeof(TSynCommon)); //struktura s promennymi pro komunikaci mezi castmi prekladace
    Tarray *arr = (Tarray *) malloc(sizeof(Tarray)); //struktura pole pro skener
    TBuffer *buffer = (TBuffer *) malloc(sizeof(TBuffer)); //buffer pro vraceni lookahead tokenu
    TSymtables_stack *local_tables = (TSymtables_stack *) malloc(sizeof(TSymtables_stack));
    Tsymbol_table *symtab_local = symtab_init(TS_SIZE);

    if(sa_vars == NULL || arr == NULL || buffer == NULL || local_tables == NULL || symtab_local == NULL) //neuspesna alokace
    { //dealokace
        free(sa_vars);
        free(arr);
        free(buffer);
        free(local_tables);
        symtab_free(symtab_local);
        return NULL;
    }

    if(arr_init(arr) == ERR_INTERNAL || !init_ts_fun(sa_vars)) //neuspesna alokace
    { //dealokace
        if(arr_init(arr) != ERR_INTERNAL)
            arr_free(arr);
        free(sa_vars);
        free(arr);
        free(buffer);
        free(local_tables);
        symtab_free(symtab_local);
        return NULL;
    }
    buffer_init(buffer);
    TS_stack_init(local_tables);

    sa_vars->local_tables = local_tables;
    sa_vars->buffer = buffer;
    sa_vars->arr = arr;
    sa_vars->boolean = false; //vychozi stav

    TS_push(sa_vars->local_tables, symtab_local);
    return sa_vars;
}

void dealloc_sa(TSynCommon *sa_vars)
{
    arr_free(sa_vars->arr);
    free(sa_vars->arr);

    TS_stack_free(sa_vars->local_tables);
    free(sa_vars->local_tables);

    delete_buffer(sa_vars->buffer);
    free(sa_vars->buffer);

    symtab_free(sa_vars->ts_fun);

    free(sa_vars);
}

void TS_stack_free(TSymtables_stack *ts_stack)
{
    while (ts_stack->top != NULL)
    {
        symtab_free(TS_pop(ts_stack));
    }
}

bool check_num_of_params(Tsymbol_table *ts, Ttoken *t, long num_of_params)
{
    Tsymbol_table_item *item = symtab_find(ts, t->attribute);
    if(item == NULL) //nenalezeno
        return false;

    if(item->num_of_params == num_of_params)
        return true;
    else if(item->num_of_params == ZERO_TO_INF && num_of_params >= 0) //specialni pripad pro fci print
        return true;
    else
        return false;
}