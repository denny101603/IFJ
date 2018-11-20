

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
*	@file code_gen.c
*	@author Matej Jelinek, Jan Carba
*	@brief soubor s definicemi funkci ze code_gen.h
*/

#include "code_gen.h"
#include "seman.h"
#include <stdio.h>
#include <stdbool.h>

void gen_defvar(TThreeAC *instruct);

void gen_move(TThreeAC *instruct);

void gen_defmove_const(TThreeAC *instruct);

void gen_createframe(TThreeAC *instruct);

void gen_pushframe(TThreeAC *instruct);

void gen_popframe(TThreeAC *instruct);

void gen_loadparam(TThreeAC *instruct);

void gen_push(TThreeAC *instruct);

void gen_pop(TThreeAC *instruct);

void gen_add(TThreeAC *instruct);

void gen_sub(TThreeAC *instruct);

void gen_mul(TThreeAC *instruct);

void gen_div(TThreeAC *instruct);

void gen_call(TThreeAC *instruct);

void gen_return(TThreeAC *instruct);

void gen_int2float(TThreeAC *instruct);

void gen_float2int(TThreeAC *instruct);

void gen_int2char(TThreeAC *instruct);

void gen_concat(TThreeAC *instruct);

void gen_setchar(TThreeAC *instruct);

void gen_isint(TThreeAC *instruct);

void gen_isfloat(TThreeAC *instruct);

void gen_isstring(TThreeAC *instruct);

void gen_isbool(TThreeAC *instruct);

void gen_lable(TThreeAC *instruct);

void gen_deffunc(TThreeAC *instruct);

void gen_jump(TThreeAC *instruct);

void gen_jumpifeq(TThreeAC *instruct);

void gen_jumpifneq(TThreeAC *instruct);

void gen_jumpifgt(TThreeAC *instruct);

void gen_jumpiflt(TThreeAC *instruct);

void gen_dprint(TThreeAC *instruct);

void gen_eq(TThreeAC *instruct);

void gen_gt(TThreeAC *instruct);

void gen_lt(TThreeAC *instruct);

void gen_gteq(TThreeAC *instruct);

void gen_lteq(TThreeAC *instruct);

void gen_neq(TThreeAC *instruct);

void GEN_start(TTacList *list)
{
    TThreeAC *I2 = TAC_remove(list);
    while (I2 != NULL)
    {
        switch (I2->name) {
            case DEFVAR:
                gen_defvar(I2);
                break;
            case MOVE:
                gen_move(I2);
                break;
            case DEFMOVE:
                gen_defmove_const(I2);
                break;
            case CREATEFRAME:
                gen_createframe(I2);
                break;
            case PUSHFRAME:
                gen_pushframe(I2);
                break;
            case POPFRAME:
                gen_popframe(I2);
                break;
            case LOADPARAM:
                gen_loadparam(I2);
                break;
            case PUSH:
                gen_push(I2);
                break;
            case POP:
                gen_pop(I2);
                break;
            case ADD:
                gen_add(I2);
                break;
            case SUB:
                gen_sub(I2);
                break;
            case MUL:
                gen_mul(I2);
                break;
            case DIV:
                gen_div(I2);
                break;
            case CALL:
                gen_call(I2);
                break;
            case RETURN:
                gen_return(I2);
                break;
            case INT2FLOAT:
                gen_int2float(I2);
                break;
            case FLOAT2INT:
                gen_float2int(I2);
                break;
            case INT2CHAR:
                gen_int2char(I2);
                break;
            case CONCAT:
                gen_concat(I2);
                break;
            case SETCHAR:
                gen_setchar(I2);
                break;
            case ISINT:
                gen_isint(I2);
                break;
            case ISFLOAT:
                gen_isfloat(I2);
                break;
            case ISSTRING:
                gen_isstring(I2);
                break;
            case ISBOOL:
                gen_isbool(I2);
                break;
            case LABLE:
                gen_lable(I2);
                break;
            case DEFFUNC:
                gen_deffunc(I2);
                break;
            case JUMP:
                gen_jump(I2);
                break;
            case JUMPIFEQ:
                gen_jumpifeq(I2);
                break;
            case JUMPIFNEQ:
                gen_jumpifneq(I2);
                break;
            case JUMPIFGT:
                gen_jumpifgt(I2);
                break;
            case JUMPIFLT:
                gen_jumpiflt(I2);
                break;
            case DPRINT:
                gen_dprint(I2);
                break;
            case EQ:
                gen_eq(I2);
                break;
            case GT:
                gen_gt(I2);
                break;
            case LT:
                gen_lt(I2);
                break;
            case GTEQ:
                gen_gteq(I2);
                break;
            case LTEQ:
                gen_lteq(I2);
                break;
            case NEQ:
                gen_neq(I2);
                break;
        }
        I2 = TAC_remove(list);
    }
}