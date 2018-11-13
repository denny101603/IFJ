//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAVO_H
#define IFJ2018_SAVO_H


#include "fsm.h"
#include "err_codes.h"
#include "sax.h"

#define TERMINUS 666 //Proč zrovna toto? Protoze nikdy nebudeme mit 666 stavu. Pokud ano, satan nam pomahej...
#define NUM_OF_RULES 87
#define RULE_LENGTH 3

/**
 * @brief Struktura prvku v zasobniku TStack.
 * @param data Nosic informace typu *token.
 * @param next, prev Ukazatele dopredu/dozadu.
 * @author Jan Beran
 */
typedef struct StackElem{
    Ttoken *data; //data == *token
    struct StackElem *next; //ukazatel na nasledujici prvek
    struct StackElem *prev; //ukazatel na predchozi prvek
}TStackElem;

/**
 * @brief Struktura zasobniku. V tomto provedeni obsahuje pouze ukazatel na vrchol zasobniku.
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
char get_action(Ttoken *input_token, Ttoken *stack_token);

/**
 * @brief Funkce, ktera zjistuje, jestli nacteny token muze byt ukoncujici token vyrazu.
 * @param token Aktualni ukazatel na token.
 * @return true, pokud je token ukoncovaci, false, pokud neni.
 */
bool is_terminus(Ttoken *token);

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
Ttoken *action_push(Ttoken *input_token, TStack *stack); //=

/**
 * @brief Funkce simuluje akci < z precedencni tabulky
 * @paragraph Pravidlo <: Zamen a za < a a udelej pravidlo =
 * @param input_token
 * @param stack
 * @return
 */
Ttoken *action_change(Ttoken *input_token, TStack *stack); //<

/**
 * @brief Funkce simuluje akci > z precedencni tabulky
 * @paragraph Pravidlo >:  pokud je na zasobniku < XYZ a existuje pravidlo p: A -> XYZ, proveď redukci <XYZ -> A. Jinak chyba.
 * @param stack
 * @return
 */
bool action_reduce(TStack *stack); //>

/**
 *
 * @param stack
 * @return
 */
int action_err(TStack *stack);

/**
 *
 * @param stack
 * @return
 */
int find_rule(TStack *stack);

/**
 *
 * @param rule
 * @param stack
 */
void execute_rule(int rule, TStack *stack);

/**
 *
 * @param input_token
 * @return
 */
bool savo(TSynCommon *sa_vars);

#endif //IFJ2018_SAVO_H