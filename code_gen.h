
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
*	@file code_gen.h
*	@author Matej Jelinek, Jan Carba
*	@brief soubor s definicemi funkci ze code_gen.h
*/

#ifndef IFJ2018_CODE_GEN_H
#define IFJ2018_CODE_GEN_H/**/

#include "seman.h"
#include "stdio.h"
#include "stdbool.h"
#include <string.h>

/**
 * @brief funkce pro zacatek generovani kodu
 * @author Matej Jelinek, Jan Carba
 * @param list seznam triadresnych instrukci ke generaci kodu
 */
void GEN_start(TTacList *list);

/**
 * @brief na stdou vlozi podobu triadresne instrukce v ifjcode2018
 * @author Matej Jelinek, Jan Carba
 * @param instruct triadresna instrukce k prevedeni na ifjcode2018
 * @return true pokud se vse povede (alokace apod.), jinak false
 */
void pream();

void gen_loadparam_def(TThreeAC *instruct);

void gen_add_def(TThreeAC *instruct);

void gen_gteq_def(TThreeAC *instruct);

void gen_jumpifeq_def(TThreeAC *instruct);

void gen_defvar(TThreeAC *instruct);

void gen_move(TThreeAC *instruct);

void gen_defmove_const( TThreeAC *instruct);

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

char *whatType(int x);

#endif //IFJ2018_CODE_GEN_H