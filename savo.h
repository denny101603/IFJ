//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAVO_H
#define IFJ2018_SAVO_H


#include "fsm.h"
#include "err_codes.h"

#define TERMINUS 666
#define NUM_OF_RULES 87
#define RULE_LENGTH 3

/**
 * @brief Tabulka pravidel pouzivana funkci get_action.
 * @author Matej Jelinek
 */
int rules[NUM_OF_RULES][RULE_LENGTH] = {
        {EXPRESSION, 0,0},
        {EXPRESSION, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, EXPRESSION},
        {LEFT_BRACKET, EXPRESSION, RIGHT_BRACKET},
        {ID_2, 0, 0},
        {EXPRESSION, OP_PLUS, ID_2},
        {ID_2, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, ID_2},
        {ID_2, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, ID_2},
        {ID_2, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, ID_2},
        {ID_2, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, ID_2},
        {ID_2, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, ID_2},
        {ID_2, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, ID_2},
        {ID_2, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, ID_2},
        {ID_2, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, ID_2},
        {ID_2, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, ID_2},
        {ID_2, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, INTEGER},
        {INTEGER, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, INTEGER},
        {INTEGER, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, INTEGER},
        {INTEGER, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, INTEGER},
        {INTEGER, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, INTEGER},
        {INTEGER, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, INTEGER},
        {INTEGER, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, INTEGER},
        {INTEGER, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, INTEGER},
        {INTEGER, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, INTEGER},
        {INTEGER, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, INTEGER},
        {INTEGER, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, FLOAT_2},
        {FLOAT_2, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, FLOAT_2},
        {FLOAT_2, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, FLOAT_2},
        {FLOAT_2, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, FLOAT_2},
        {FLOAT_2, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, FLOAT_2},
        {FLOAT_2, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, FLOAT_2},
        {FLOAT_2, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, FLOAT_2},
        {FLOAT_2, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, FLOAT_2},
        {FLOAT_2, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, FLOAT_2},
        {FLOAT_2, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, FLOAT_2},
        {FLOAT_2, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, STRING_1},
        {STRING_1, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MORE_1, STRING_1},
        {STRING_1, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, STRING_1},
        {STRING_1, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, STRING_1},
        {STRING_1, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, STRING_1},
        {STRING_1, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, STRING_1},
        {STRING_1, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, STRING_1},
        {STRING_1, OP_NOT_EQ_1, EXPRESSION},
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
//TODO ALL Zkontrolovat to po Janu Carbovi :)

/**
 * @brief Struktura prvku v zasobniku TStack
 *
 * @author Jan Beran
 */
typedef struct StackElem{
    Ttoken *data; //data == *token
    struct StackElem *next; //ukazatel na nasledujici prvek
    struct StackElem *prev; //ukazatel na predchozi prvek
}TStackElem;

/**
 * @brief Struktura Zasobniku. V tomto provedeni obsahuje pouze ukazatel na vrchol zasobniku.
 * @author Jan Beran
 */
 typedef struct Stack{
    TStackElem *top;
}TStack;

/**
 * @brief Funkce inicializuje zasobnik typu TStack tak, že do něj vlozi prvni prvek, kterym je token s typem BOTTOM_TOKEN
 * @author Jan Beran
 * @param in stack stack k inicializaci
 * @return true
 */
bool stack_init(TStack *stack);

/**
 * @brief Funkce, ktera pridava token na vrchol zasobniku. Token nejprve obali do struktury TStackElem a pote ho vlozi.
 * @param stack Ukazatel na zasobik, kam se ma token vlozit.
 * @param token Ukazatel na token, ktery se ma vlozit na zasobik-
 * @return true nebo false, podle vysledku alokace.
 */
bool push(TStack *stack, Ttoken *token);

/**
 * @brief Funkce popne vrchni polozku TSTackElem ze zasobniku stack a vrati hodnotu tokenu.
 * @author Jan Beran
 * @param Stack zasobnik, ze ktereho bereme hodnotu
 * @return Popnuty token.
 */
Ttoken *pop(TStack *stack);

/**
 * @brief Funkce vymaze zasobnik vcetne prvni polozky.
 * @author Jan Beran
 * @param stack Zasobnik k vymazani.
 */
void delete_stack(TStack *stack);

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
 * @paragraph Pravidlo <:
 * @param input_token
 * @param stack
 * @return
 */
Ttoken action_change(Ttoken input_token, TStack *stack); //<

/**
 * @brief Funkce simuluje akci > z precedencni tabulky
 * @paragraph
 * @param stack
 * @return
 */
bool action_reduce(TStack *stack); //>
int action_err(TStack *stack);

int find_rule(TStack *stack);
void execute_rule(int rule, TStack *stack);

bool savo(Ttoken *input_token);

#endif //IFJ2018_SAVO_H