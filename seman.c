

#include "seman.h"

/**
 *  @brief Vycet vsech datovych typu
*   @author Matej Jelinek
 */
enum data_type{
    INT,
    FLOAT,
    CHAR,
    STRING
};

/**
 *  @brief Vycet vsech triadresnych instrukci
*   @author Matej Jelinek
 */
enum instructions{
    DEFVAR,
    MOVE,
    CREATEFRAME,
    PUSHFRAME,
    POPFRAME,
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    INPUTI,
    INPUTS,
    INPUTF,
    PRINT,
    LENGTH,
    SUBSTR,
    ORD,
    CHR,
    CALL,
    RETURN,
    INT2FLOAT,
    FLOAT2INT,
    INT2CHAR,
    CONCAT,
    SETCHAR,
    ISINT,
    ISFLOAT,
    ISSTRING,
    ISBOOL,
    LABLE,
    JUMP,
    JUMPIFEQ,
    JUMPIFNEQ,
    JUMPIFGT,
    JUMPIFLT,
    DPRINT
};

TTacList *TAC_init()
{
    TTacList *new = malloc(sizeof(TTacList));
    if(new == NULL)
        return NULL;
    new->first = NULL;
    new->last = NULL;
}

void TAC_insert_post(TTacList *list, TThreeAC *elem, TThreeAC *inserted)
{

}

TThreeAC *TAC_remove_post(TTacList *list, TThreeAC *elem)
{

}

void TAC_delete_list(TTacList *list)
{

}
