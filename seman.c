
/***************************************
* 	IFJ projekt 2018                   *
* 						               *
*	Autori:			                   *
*	Jan Beran (xberan43)	           *
*	Daniel Bubenicek (xbuben05)	       *
*	Jan Carba (xcarba00)		       *
*	Matej Jelinek (xjelen49)	       *
*                                      *
***************************************/
/**
*	@file seman.c
*	@author Matěj Jelínek, Jan Beran
*	@brief soubor s definicemi funkci ze seman.h
 *	v1.2: Vsechny funkce vytvorene Janem Beranem otestovane v simulaci (nemam semanticky generator, ktery by je otestoval naostro).
 *	      V simulovanych testech vsechno fungovalo dobre.
*/
#include "seman.h"


TTacList *TAC_init()
{
    TTacList *new = malloc(sizeof(TTacList));
    if(new == NULL)
        return NULL; //err
    new->first = NULL;
    new->last = NULL;
    return new;
}


void TAC_insert(TTacList *list, TThreeAC *inserted)
{
    if (list == NULL || inserted == NULL)//err
        return;
    //prazdny seznam
    if(list->first == NULL && list->last == NULL) // pokud se rovna NULL jen jeden, nekdo si s tim hral a je to chyba (neosetrovano, pri pouzivani fci se to nestane)
    {
       inserted->next = NULL;
       inserted->prev = NULL; //pro jistotu
       list->first = inserted;
       list->last = inserted;
    }
    else
    {
     inserted->next = NULL;
     list->last->next = inserted;
     inserted->prev = list->last;
     list->last = inserted;
    }
}

TThreeAC *TAC_remove(TTacList *list)
{
    if (list == NULL)
        return NULL;
    if(list->last == NULL && list->first == NULL) //zadny prvek. Pokud je NULL jen jeden => ERR
    {
        return NULL;
    }
    //alespon jeden prvek
    TThreeAC *ret = list->last;
    if(list->last == list->first) //jediny prvek => last se do NULL hodi sam, firstu je treba pomoci :)
        list->first = NULL;
    list->last = list->last->prev; // if list->last->prev == NULL, neni problem, stale OK
    if(list->last != NULL) //pokud NULL neni, korektne zaslepim
        list->last->next = NULL;
    ret->prev = NULL; //odpojeni od listu
    ret->next = NULL; //jistota, neni nutne
    return ret;
}


void TAC_insert_post(TTacList *list, TThreeAC *elem, TThreeAC *inserted)
{
    //neni seznam, neni urceno, za co vkladat, neni urceno co vkladat, seznam je prazdny -> ERR
    // (list->last == NULL netestovano, first a last jsou bud zaroven NULL nebo zaroven NULL nejsou pri pouziti fci) //todo denny, dokaz opak :)
    if(list == NULL || elem == NULL || inserted == NULL || list->first == NULL)
        return;

    inserted->next = elem->next;
    if(elem->next != NULL)
        elem->next->prev = inserted;
    elem->next = inserted;
    inserted->prev = elem;
    if(elem == list->last)
         list->last = inserted;

}

TThreeAC *TAC_remove_post(TTacList *list, TThreeAC *elem)
{
    //neexistujici seznam, prvek, prazdny seznam, jednoprvkovy seznam -> ERR
    if(list == NULL || elem == NULL || list->first == NULL || list->first->next == NULL)
        return  NULL;
    if(elem->next == NULL) //elem je posledni prvek listu, neni co odstranit. Tohle ERR neni, jen to je hovadina :D
        return NULL;
    TThreeAC *ret = NULL;
    ret = elem->next;
    if(elem->next->next == NULL) //odstranuji posledni prvek
    {
        list->last = elem;
        list->last->next = NULL;
        ret->next = NULL;
        ret->prev = NULL;
    }
    else //odstranuji prvek kdesi uprostred, musim napojit
    {
        elem->next = elem->next->next;
        elem->next->prev = elem;
        ret->next = NULL;
        ret->prev = NULL;
    }
    return ret;
}

void TAC_delete_list(TTacList *list)
{
    while(list->first != NULL)
    {
        /**Ladici vypisy, pred pouzitim zakomentovat**/
       /* TThreeAC *item = list->first;
        while(item != NULL)
        {
            printf("%d\n", item->name);
            item = item->next;
        }
        printf("_________________________\n");*/
        /**konec ladiciho vypisu**/
        free(TAC_remove(list));
    }

}


/*typedef struct tac_buffer{
    Toperand *top;
} Ttac_buffer;*/

Ttac_buffer *op_buffer_init()
{
    Ttac_buffer *buffer = malloc(sizeof(Ttac_buffer));
    if(buffer == NULL)
        return NULL;
    buffer->top = NULL;
    return buffer;
}

void op_push(Ttac_buffer *buffer, Toperand *operand)
{
    if(buffer == NULL || operand == NULL)
        return;

    TTAC_Elem *elem = malloc(sizeof(TTAC_Elem));
    if(elem == NULL)
        return;

    elem->operand = operand;

    elem->prev = buffer->top;
    buffer->top = elem;
}

Toperand *op_pop(Ttac_buffer *buffer)
{
    TTAC_Elem *elem = buffer->top;
    buffer->top = buffer->top->prev;
    Toperand *ret = elem->operand;
    free(elem);
    return ret;
}

void op_delete_buffer(Ttac_buffer *buffer)
{
    Toperand *temp = NULL;
    while(buffer->top != NULL)
    {
        temp = op_pop(buffer);
        free(temp);
    }
    free(buffer);
}


bool instruc_init(TTacList *list, int name, Toperand *destination, Toperand *op1, Toperand *op2)
{
    TThreeAC *I1 = malloc(sizeof(TThreeAC));
    if(I1 == NULL)
        return false;
    I1->name = name;
    I1->destination = destination;
    I1->op_1 = op1;
    I1->op_2 = op2;
    I1->prev = NULL;
    I1->next = NULL;
    TAC_insert(list, I1);
    return true;
}

bool tac_defvar(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, DEFVAR, NULL, op1, NULL))
        return true;
    return false;
}
bool tac_move(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, MOVE, dest, op1, NULL))
        return true;
    return false;
}

bool tac_createframe(TTacList *list)
{
    if(instruc_init(list, CREATEFRAME, NULL, NULL, NULL))
        return true;
    return false;
}

bool tac_pushframe(TTacList *list)
{
    if(instruc_init(list, PUSHFRAME, NULL, NULL, NULL))
        return true;
    return false;
}

bool tac_popframe(TTacList *list)
{
    if(instruc_init(list, POPFRAME, NULL, NULL, NULL))
        return true;
    return false;
}

bool tac_loadparam(TTacList *list, Toperand *dest)
{
    if (instruc_init(list, LOADPARAM, dest, NULL, NULL))
        return true;
    return false;
}

bool tac_push(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, PUSH, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_pop(TTacList *list, Toperand *dest)
{
    if (instruc_init(list, POP, dest, NULL, NULL))
        return true;
    return false;
}

bool tac_add(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, ADD, dest, op1, op2))
        return true;
    return false;
}

bool tac_sub(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, SUB, dest, op1, op2))
        return true;
    return false;
}

bool tac_mul(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, MUL, dest, op1, op2))
        return true;
    return false;
}

bool tac_div(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, DIV, dest, op1, op2))
        return true;
    return false;
}

bool tac_inputi(TTacList *list, Toperand *dest)
{
    if (instruc_init(list, INPUTI, dest, NULL, NULL))
        return true;
    return false;
}

bool tac_inputs(TTacList *list, Toperand *dest)
{
    if (instruc_init(list, INPUTS, dest, NULL, NULL))
        return true;
    return false;
}

bool tac_inputf(TTacList *list, Toperand *dest)
{
    if (instruc_init(list, INPUTF, dest, NULL, NULL))
        return true;
    return false;
}

//TODO print

bool tac_length(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, LENGTH, dest, op1, NULL))
        return true;
    return false;
}

//TODO substr

bool tac_ord(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, ORD, dest, op1, op2))
        return true;
    return false;
}

bool tac_chr(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, CHR, dest, op1, NULL))
        return true;
    return false;
}

bool tac_call(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, CALL, dest, op1, NULL))
        return true;
    return false;
}

bool tac_return(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, RETURN, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_int2float(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, INT2FLOAT, dest, op1, NULL))
        return true;
    return false;
}

bool tac_float2int(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, FLOAT2INT, dest, op1, NULL))
        return true;
    return false;
}

bool tac_int2char(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, INT2CHAR, dest, op1, NULL))
        return true;
    return false;
}

bool tac_concat(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, CONCAT, dest, op1, op2))
        return true;
    return false;
}

bool tac_setchar(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, SETCHAR, dest, op1, op2))
        return true;
    return false;
}

bool tac_isint(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, ISINT, dest, op1, NULL))
        return true;
    return false;
}

bool tac_isfloat(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, ISFLOAT, dest, op1, NULL))
        return true;
    return false;
}

bool tac_isstring(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, ISSTRING, dest, op1, NULL))
        return true;
    return false;
}

bool tac_isbool(TTacList *list, Toperand *dest, Toperand *op1)
{
    if(instruc_init(list, ISBOOL, dest, op1, NULL))
        return true;
    return false;
}

bool tac_lable(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, LABLE, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_deffunc(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, DEFFUNC, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_jump(TTacList *list, Toperand *op1)
{
    if (instruc_init(list, JUMP, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_jumpifeq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, JUMPIFEQ, dest, op1, op2))
        return true;
    return false;
}

bool tac_jumpifneq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, JUMPIFNEQ, dest, op1, op2))
        return true;
    return false;
}

bool tac_jumpifgt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, JUMPIFGT, dest, op1, op2))
        return true;
    return false;
}

bool tac_jumpiflt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, JUMPIFLT, dest, op1, op2))
        return true;
    return false;
}

bool tac_dprint(TTacList *list, Toperand *op1)
{
    if(instruc_init(list, DPRINT, NULL, op1, NULL))
        return true;
    return false;
}

bool tac_eq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, EQ, dest, op1, op2))
        return true;
    return false;
}

bool tac_gt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, GT, dest, op1, op2))
        return true;
    return false;
}

bool tac_lt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, LT, dest, op1, op2))
        return true;
    return false;
}

bool tac_gteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, GTEQ, dest, op1, op2))
        return true;
    return false;
}

bool tac_lteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, LTEQ, dest, op1, op2))
        return true;
    return false;
}

bool tac_neq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2)
{
    if(instruc_init(list, NEQ, dest, op1, op2))
        return true;
    return false;
}

Toperand *op_init(int type, char *name)
{
    Toperand *op = (Toperand *) malloc(sizeof(Toperand));
    if(op == NULL)
        return NULL;

    op->type = type;
    op->name = name;
    return op;
}