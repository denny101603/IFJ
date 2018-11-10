//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAVO_H
#define IFJ2018_SAVO_H

#endif //IFJ2018_SAVO_H
#include "fsm.h"
#include "err_codes.h"
#define TERMINUS 666
#define NUM_OF_RULES 53
#define RULE_LENGTH 3
/**
 * @brief tabulka pravidel
 * @author Matej Jelinek
 */
int rules[NUM_OF_RULES][RULE_LENGTH] = {
        {EXPRESSION, 0,0},
        {EXPRESSION, OP_PLUS, EXPRESSION},


};

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


bool stack_init(TStack *stack);


bool push(TStack *stack, Ttoken *token);

Ttoken *pop(TStack *stack);

void delete_stack(TStack *stack)
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


/**
 * @brief Akce vykona operaci push a tim simuluje akci = z precedencni tabulky
 * @param input_token token, ktery ma byt pushnut na stack
 * @param stack Aktualni zasobik
 * @return Novy token typu Ttoken
 */
Ttoken action_push(Ttoken input_token, TStack *stack); //=

/**
 * @brief Funkce simuluje akci < z precedencni tabulky
 * @param input_token
 * @param stack
 * @return
 */
Ttoken action_change(Ttoken input_token, TStack *stack); //<
bool action_reduce(TStack *stack); //>
int action_err(TStack *stack);

int find_rule(TStack *stack);
void execute_rule(int rule, TStack *stack);
