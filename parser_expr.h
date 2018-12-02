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
*	@file savo.h
*	@author Jan Beran
*	@brief knihovna pro potreby Syntaktickeho AnalyzatOru Vyrazu (SAVO)
 *	v1.2: Vsechny funkce deklarovany, behem refaktorizace sava.c budou doplneny komentare a dalsi dokumentace.
*/
#ifndef IFJ2018_PARSER_EXPR_H
#define IFJ2018_PARSER_EXPR_H

//
#include "scanner.h"
#include "err_codes.h"
#include "parser.h"
#include "seman.h"

//#define TERMINUS 666 //Proč zrovna toto? Protoze nikdy nebudeme mit 666 stavu. Pokud ano, satan nam pomahej...


/**
 * @brief Struktura prvku v zasobniku TStack.
 * @param data Nosic informace typu *token.
 * @param next, prev Ukazatele dopredu/dozadu.
 * @author Jan Beran
 */
typedef struct StackElem{
    Ttoken *data; //data == *token
    Toperand *operand; //operand
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
  * @brief Funkce vraci jmeno pro docasnou promennou. Nutne pri sestavovani vysledneho kodu
  * @return vygenerovane jmeno pro promennou typu char *.
  */
 char *savo_name_generator(Tgarbage_collector *collector);
/**
 * @brief Funkce inicializuje zasobnik typu TStack tak, že do něj vlozi prvni prvek, kterym je token s typem BOTTOM_TOKEN
 * @author Jan Beran
 * @param in stack stack k inicializaci
 * @return true
 */
TStack *stack_init(TSynCommon *sa_vars);

/**
 * @brief Funkce, ktera pridava token na vrchol zasobniku. Token nejprve obali do struktury TStackElem a pote ho vlozi. Zaroven do struktury TStackElem zahrne operand op
 * @param stack Ukazatel na zasobik, kam se ma token vlozit.
 * @param input_token Ukazatel na token, ktery se ma vlozit na zasobik.
 * @param stack_elem ukazatel na polozku s tokenem, ZA ktery se ma prvek vlozit
 * @param op operand, spjaty s tokenem
 * @return true nebo false, podle vysledku alokace.
 */
bool push(TStack *stack, TStackElem *stack_elem, Ttoken *input_token, Toperand *op, TSynCommon *sa_vars);

/**
 * @brief Funkce popne vrchni polozku TSTackElem ze zasobniku stack a vrati hodnotu tokenu.
 * @author Jan Beran
 * @param Stack zasobnik, ze ktereho bereme hodnotu
 * @return Popnuty token.
 */
Ttoken *pop(TStack *stack);

/**
 * @brief Funkce popne vrchni polozku TSTackElem ze zasobniku stack a vrati hodnotu tokenu. zaroven do promenne op preda hodnotu operandu
 * @author Jan Beran
 * @param Stack zasobnik, ze ktereho bereme hodnotu
 * @param op ukazatel na operand, do ktereho davame hodnotu operandu
 * @return Popnuty token.
 */
Ttoken *pop_extended(TStack *stack, Toperand *op);
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
 * @return Odkaz na dany polozku s danym tokenem
 */
TStackElem *get_first_terminal(TStack *stack);

/**
 * @brief Funkce nacte token z elementu elem a vrati odkaz na nej (nemodifikuje, pouze vraci odkaz)
 * @param elem Element zasobniku, ze ktereho potrebujeme token
 * @return okazatel na dany token
 * @warning Funkce nic neoveruje, v pripade elem == NULL nedefinovane chovani (err).
 */
Ttoken *get_token_from_elem(TStackElem *elem);

/**
 * @brief Akce vykona operaci push a tim simuluje akci = z precedencni tabulky
 * @param input_token token, ktery ma byt pushnut na stack
 * @param stack Aktualni zasobik
 * @return Novy token typu Ttoken
 */
Ttoken *action_push(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer); //=

/**
 * @brief Funkce zjisti, zda je dany token tzv "pseudotoken", token vytvoreny pro potreby sy. anal. vyrazu (SAVO).
 * @param token vstupni token
 * @return true, pokud token je pseudotoken, false, pokud se jedna o standardní token (získaný ze scanneru).
 */
bool is_pseudotoken(Ttoken *token);
/**
 * @brief Funkce simuluje akci < z precedencni tabulky
 * @paragraph Pravidlo <: za prvni terminal od topu zasobniku pushni <, pote pushni input token na vrchol a vezmi dalsi ze vstupu
 * @author Jan Beran
 * @param input_token token, ktery ma byt pushnut na zasobnik
 * @param stack zasobnik, se kterym ma fce pracovat
 * @param sa_vars spolecne promenne syn. analyzatoru
 * @param internal_buffer interni buffer pro ukladani prichozich tokenu
 * @return novy vstupni token
 */
Ttoken *action_change(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer); //<

/**
 * @brief Funkce simuluje akci > z precedencni tabulky
 * @paragraph Pravidlo >:  pokud je na zasobniku <XYZ && existuje pravidlo A -> XYZ,
 * zamen XYZ za A. Jinak se jedna o chybu
 * @author Jan Beran
 * @param stack zasobnik, se kterym ma fce pracovat
 * @param sa_vars spolecne promenne syn. analyzatoru
 * @param internal_buffer interni buffer pro ukladani prichozich tokenu
 * @return T/F podle toho, zda se akce podarila (naslo se pravidlo) nebo ne.
 */
bool action_reduce(TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer); //>

/**
 *@brief Funkce kopíruje obsah bufferu src do bufferu dst
 * @param src source buffer
 * @param dst destination buffer
 * @return t/f, podle úspěšnosti
 */
bool copy_buffer(TBuffer *src, TBuffer *dst, TSynCommon *sa_vars);

/**
 *@brief Funkce, ktera resi error v savu. Funkce dealokuje lokalni zasobnik,
 * do sa_vars nastavi hodnotu error a zkopiruje interni buffer do spolecneho
 * @author Jan Beran
 * @param stack interni zasobnik pro SA vyrazu
 * @param sa_vars spolecne promenne sava a saxu
 * @param error typ erroru
 * @param internal_buffer interni buffer pro uchovavani tokenu v pripade erroru
 * @note ERR_SYN se vraci pouze jako navrat false z funkce savo(), pres action_err se nepropaguje. Vysvetleni u definice
 * @return ciselna hodnota erroru
 */
int action_err(TStack *stack, TSynCommon *sa_vars, int error, TBuffer *internal_buffer);

/**
 * @brief Funkce hleda pravidlo v tabulce pravidel na zaklade posloupnosti XYZ z pravidla <
 * @author Jan Beran
 * @param stack zasobnik, se kterym se ma pracovat
 * @return cislo pravidla v tabulce pravidel, -1 pokud nenalezeno
 */
int find_rule(TStack *stack);

/**
 *@brief Funkce provadi pravidlo, ziskane z precedencni tabulky pomoci fce find_rule. Provadi jak syntakticke akce (predkontrola a samotna kontrola, ktera je destruktivní), tak semanticke akce.
 * @param rule pravidlo, ktere ma byt provedeno
 * @param stack zasobnik syntakticke analyzy vyrazu
 * @param sa_vars spolecne promenne syn. analyzy
 * @param internal_buffer interni buffer pro zalohu tokenu v pripade nepovedene analyzy a nutnosti je vratit
 */
bool execute_rule(int rule, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer);

/**
 *@brief Zakladni funkce, kterou se zapiná syntakticka analyza vyrazu.
 * @author Jan Beran
 * @param sa_vars spolecne promenne pro oba syntakticke analyzatory
 * @return true/false podle uspesnosti analyzy vyrazu
 */
bool savo(TSynCommon *sa_vars);

#endif //IFJ2018_PARSER_EXPR_H