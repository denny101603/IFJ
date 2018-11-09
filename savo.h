//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAVO_H
#define IFJ2018_SAVO_H

#endif //IFJ2018_SAVO_H
#include "fsm.h"
#define TERMINUS 666
/**
 * @brief Precedencni tbaulka pro syntaktickou analyzu vyrazu.
 * @author Jan Carba
 */
char prec_table[15][15] = {
        /*           + 	  -   *	  /	  (	  )	 id	 con  <   >  <=	  >=  !=  ==   $ */
        /* + */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* - */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* * */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* / */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* ( */    {'<','<','<','<','<','=','<','<','<','<','<','<','<','<','?'},
        /* ) */    {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* id */   {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* cons */ {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* < */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* > */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* <= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* >= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* != */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* == */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* $ */    {'<','<','<','<','<','?','<','<','?','?','?','?','?','?','?'}
};
//TODO Zkontrolovat to po Janu Carbovi :)
//todo
typedef struct StackElem{
    Ttoken *data;
    struct StackElem *next;
    struct StackElem *prev;
}TStackElem;
/**
 * @brief Struktura
 */typedef struct Stack{
    TStackElem *top;
}TStack;




/**
 * @brief Funkce pro zjisteni, co se ma provadet v dalsim kroku syntakticke analyzy vyrazu.
 * @author Jan Beran
 * @param input_token Token nacteny ze vstupu.
 * @param stack_token prvni terminalni token na zasobniku.
 * @return Vraci znak reprezentujici akci (<, >, =, ?)
 */


char get_action(Ttoken input_token, Ttoken stack_token);

/**
 * @brief Funkce, ktera zjistuje, jestli nacteny token muze byt ukoncujici token vyrazu.
 * @param token Aktualni token.
 * @return true, pokud je token ukoncovaci, false, pokud neni.
 */
bool is_terminus(Ttoken token);

/**
 * @brief Funkce vraci prvni terminalni token ze zasobniku
 * @param stack Zasobnik, se kterym ma funkce pracovat
 * @return Odkaz na dany token
 */
Ttoken *get_first_terminal(TStack *stack);


bool stack_init(TStack *stack);