//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAX_H
#define IFJ2018_SAX_H

#include "fsm.h"
#include "symtable.h"

#define TS_SIZE 127ul //TODO domluvit se na nejake velikosti

/**
 * @brief Struktura prvku v ADT Buffer
 *
 * @author Jan Beran
 */
typedef struct BufferElem{
    Ttoken *data; //data == *token
    struct BufferElem *next; //ukazatel na nasledujici prvek
    struct BufferElem *prev; //ukazatel na predchozi prvek
}TBufferElem;

/**
 * @brief Struktura pro ADT Buffer. Kombinuje v sobe vlastnosti zasobniku a fronty (fce zasobniku je potrebna pro savo, fronta pro sax)
 * @author Jan Beran
 * @note savo pracuje s bufferem v rezimu zasobniku (LIFO), sax v rezimu fronty (FIFO).
 */
typedef struct Buffer{
    TBufferElem *top; //vrsek zasobniku = posledni vlozeny prvek
    TBufferElem *bottom; //dno zasobniku = prvni vlozeny prvek
}TBuffer;

/**
 * @brief Struktura pro polozku zasobniku lokalnich tabulek symbolu.
 * @author Jan Beran
 *
 */
typedef struct LTElem{
Tsymbol_table *data;
struct LTElem *prev;
}TLTElem; //Typ: Local Table Element

/**
 * @brief Struktura zasobniku lokalnich tabulek symbolů
 * @author Jan Beran
 *
 */
typedef struct local_tables{
    TLTElem *top; //ukazatel na posledni prvek = naposled vlozeny (viz obrazek :D )
    TLTElem *bottom; //ukazatel na prvni prvek = prvne vlozeny (viz obrazek :D )
} TSymtables_stack; //TODO denny by berry jak to pojmenovat?
//ASCII ukazka zasobniku:
//|______|________________|___|______
//|bottom|................|top|  ><sem se vkladaji nove prvky
//|______|________________|___|______

/**
 * @brief Struktura, slouzici pro komunikaci mezi scannerem, savem a saxem.
 * @authors Jan Beran, Daniel Bubenicek
 * @param arr - pro spravnou funkci skeneru
 * @param buffer buffer (vyrovnavaci pamet) pro komunikaci mezi sax a savo
 */
typedef struct SynCommon{

    Tarray *arr; //TODO by denny: tohle, tablefun i buffer by měly byt ukazatele, ne? edit: potrbuju to, takze jsem to upravil
    TBuffer *buffer;
    Tsymbol_table *ts_fun;
    int err_code; //pro uchovani pripadne chyby
   //TSymTable table_local; nahrazeno stackem
   //TODO by denny: pridat nasledujici, asi to budu potrebovat
    //TSymtables_stack *local_tables; //TODO DONE By Berry
    //puvodně bylo psano: stack *local_tables -nahrada za table_local, myslim ze jedna nestaci
} TSynCommon;

/********************************/
/* Funkce pro TBuffer */
/********************************/

/**
 * @brief Funkce inicializuje zasobnik typu TBuffer.
 * @author Jan Beran
 * @param in buffer buffer k inicializaci
 * @return true
 */
bool buffer_init(TBuffer *buffer_buffer);

/**
 * @brief Funkce, ktera pridava token na vrchol zasobniku. Token nejprve obali do struktury TBufferElem a pote ho vlozi.
 * @param buffer Ukazatel na zasobik, kam se ma token vlozit.
 * @param token Ukazatel na token, ktery se ma vlozit na zasobik.
 * @return true nebo false, podle vysledku alokace.
 */
bool buffer_push_top(TBuffer *buffer, Ttoken *token);

/**
 * @brief Funkce, ktera pridava token na dno zasobniku. Token nejprve obali do struktury TBufferElem a pote ho vlozi.
 * @author Jan Beran
 * @param buffer Ukazatel na zasobnik, kam se ma token vlozit.
 * @param token Ukazatel na token, ktery se ma vlozit na zasobik.
 * @return true nebo false, podle vysledku alokace.
 */
bool buffer_push_bottom(TBuffer *buffer, Ttoken *token);

/**
 * @brief Funkce popne vrchni polozku TBufferElem ze zasobniku buffer a vrati hodnotu tokenu.
 * @author Jan Beran
 * @param buffer zasobnik, ze ktereho bereme hodnotu
 * @return Popnuty token.
 */
Ttoken *buffer_popTop(TBuffer *buffer);

/**
 * @brief Funkce popne spodni polozku TBufferElem ze zasobniku buffer a vrati hodnotu tokenu.
 * @author Jan Beran
 * @param buffer zasobnik, ze ktereho bereme hodnotu
 * @return Popnuty token.
 */
Ttoken *buffer_popBottom(TBuffer *buffer);

/**
 * @brief Funkce vymaze zasobnik.
 * @author Jan Beran
 * @param buffer Zasobnik k vymazani.
 */
void delete_buffer(TBuffer *buffer);

/**
 * @brief Funkce vraci pravdivostni hodnotu prazdnosti bufferu.
 * @author Jan Beran
 * @return true, pokud je buffer prazdny, false, pokud neni.
 */
bool buffer_empty(TBuffer *buffer);

/**************************************/
/*Funkce pro zasobnik tabulek symbolu (T_symbol_table...)*/
/**************************************/

/**
 * @brief Funkce inicializuje zasobik typu TSymtables_stack
 * @author Jan Beran
 * @param stack zasobnik k inicializaci.
 * @return bool true/false podle toho, zda se podari inicializace.
 */
bool TS_stack_init(TSymtables_stack *stack);

/**
 * @brief Funkce vlozi prvek na vrchol zasobniku.
 * @author Jan Beran
 * @return t/f podle vysledku
 */
 bool TS_push(TSymtables_stack *stack, Tsymbol_table *table);

 /**
  * @brief Funkce popuje vrchni (top) prvek stacku.
  * @author Jan Beran
  * @param stack zasobnik, ze ktereho ma byt popnut prvek
  * @return popnuty prvek.
  */
Tsymbol_table *TS_pop(TSymtables_stack *stack);



/**
 * @brief Funkce vraci dalsi token a na zaklade toho, zda se v bufferu nachazi nejake tokeny, je bere bud z nej, nebo ze stdin pomoci scanneru.
 * @author Jan Beran, Daniel Bubenicek (arr)
 * @param arr Dennyho megahustodemonskykrutoprisne pole, ktere vsechny jenom sere :)
 * @param buffer buffer pro uchovavani tokenu
 * @return Token z prislusneho zdroje
 */
Ttoken get_next_token(Tarray *arr, TBuffer *buffer);

/**
*	@brief provadi cely preklad
*	@author Daniel Bubenicek
*	@return kod chyby/uspechu prekladu
*/
int startSA();

/**
*	@brief zkousi prelozit cast programu ktery muze stat samostatne (cast hlavniho tela)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool progr(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal deffunc (reprezentuje konstrukci definici funkce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_deffunc(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal params (reprezentuje parametry pri definici fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_params(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal nextparams (reprezentuje 2. a kazdy dalsi parametr pri definici fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_nextparams(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal bodyfce (reprezentuje telo fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_bodyfce(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal eolf (reprezentuje ukonceni konstrukci - eof nebo eol)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_eolf(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal cycl (reprezentuje konstrukci cyklu while)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_cycl(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal bodywhif (reprezentuje telo v konstrukcich if-then-else a while)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_bodywhif(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal ifthenelse (reprezentuje konstrukci if-then-else)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_ifthenelse(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal assignment (reprezentuje prikaz prirazeni)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_assignment(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal right (reprezentuje pravou stranu prikazu prirazeni)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_right(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal leftbracket (reprezentuje volitelnou levou zavorku pri volani fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_leftbracket(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal rightbracket (reprezentuje volitelnou pravou zavorku pri volani fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_rightbracket(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal args (reprezentuje argumenty pri volani fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_args(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal nextargs (reprezentuje 2. a kazdy dalsi argument pri volani fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_nextargs(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal expression (reprezentuje vyraz)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_expression(TSynCommon *sa_vars);

/**
*	@brief funkce pro neterminal callfce (reprezentuje konstrukci volani fce)
*	@author Daniel Bubenicek
*	@return true pro uspech jinak false
*/
bool nt_callfce(TSynCommon *sa_vars);




#endif //IFJ2018_SAX_H