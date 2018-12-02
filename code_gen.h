
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
*	@brief soubor s definicemi funkci z code_gen.h
*/

#ifndef IFJ2018_CODE_GEN_H
#define IFJ2018_CODE_GEN_H

#include "tac.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief funkce pro zacatek generovani kodu
 * @author Matej Jelinek, Jan Carba, Daniel Bubenicek
 * @param list seznam triadresnych instrukci ke generaci kodu
 */
void GEN_start(TTacList *list, Tgarbage_collector *collector);

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

void gen_loadparam(TThreeAC *instruct);

void gen_push(TThreeAC *instruct);

void gen_add(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_sub(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_mul(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_div(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_call(TThreeAC *instruct);

void gen_return(TThreeAC *instruct);

void gen_lable(TThreeAC *instruct);

void gen_deffunc(TThreeAC *instruct);

void gen_jump(TThreeAC *instruct);

void gen_jumpifeq(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_jumpifneq(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_dprint(TThreeAC *instruct);

void gen_eq(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_gt(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_lt(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_gteq(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_lteq(TThreeAC *instruct, Tgarbage_collector *collector);

void gen_neq(TThreeAC *instruct, Tgarbage_collector *collector);

char *whatType(int x);

#endif //IFJ2018_CODE_GEN_H