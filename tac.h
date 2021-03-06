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
*	@file tac.h
*	@author Matěj Jelínek, Jan Beran, Daniel Bubenicek
*	@brief hlavickovy soubor pro generaci triadresneho kodu
*/

#ifndef PROJECT_TAC_H
#define PROJECT_TAC_H

typedef struct gc Tgarbage_collector;

#include <stdlib.h>
#include <stdbool.h>

#define MAX_NAMES 6 //do struktury TThreeAC
/**
*	@brief struktura operandu
 *	@author Matej Jelinek
*/
typedef struct operand{
    int type; //datovy typ
    char *name; //nazev promenne
}Toperand;

/**
 *
 */
typedef struct TAC_elem{
    Toperand *operand;
    struct TAC_elem *prev;
} TTAC_Elem;

/**
 * @brief Struktura pro buffer
 * @warning 0.1 pouze vytvorena, aby nebyl problem pri prekladu
 *
 */
typedef struct tac_buffer{
    TTAC_Elem *top;
} Ttac_buffer;



/**
 * @brief Struktura pro jednu triadresnou instrukci.
 * @authors Jan Beran, Matej Jelinek
 * @warning Buffer pouzivat jen pro funkce print a substr
 **/
typedef struct three_ac{
    int name;
    Toperand *destination;
    Toperand *op_1;
    Toperand *op_2;
    char *names[MAX_NAMES]; //max pocet predavanych nazvu
    struct three_ac *next;
    struct three_ac *prev;
}TThreeAC;

void TThreeAC_delete(TThreeAC* tac);

/**
 * @brief alokuje strukturu Toperand a naplni ji
 * @author Daniel Bubenicek
 * @param type typ operandu
 * @param name jmeno promenne nebo u konstant hodnota
 * @param collector
 * @return operand, pokud se nepovede alokace tak NULL
 */
Toperand *op_init(int type, char *name, Tgarbage_collector *collector);


/**
 * @brief Struktura pro seznam triadresnych instrukci, kde instrukce na odkaze oznacenem jako "first" je prvni instrukci programu.
 * @author Jan Beran
 */
typedef struct tac_list{/**/
    TThreeAC *first;
    TThreeAC *last;
}TTacList;

/**
 * @brief Funkce pro vytvoreni a alokaci seznamu triadresnych instrukci (Konstruktor).
 * @author Jan Beran
 * @return Ukazatel na nove vytvoreny seznam typu TTacList nebo NULL pri neuspesne alokaci
 */
TTacList *TAC_init(Tgarbage_collector *collector);


/**
 * @brief Funkce vlozi prvek inserted na misto ZA prvek elem v seznamu list.
 * @author Jan Beran
 * @param elem ukazatel na prvek seznamu list, ZA ktery ma byt vlozen prvek inserted.
 * @param list ukazatel na seznam, do ktereho se bude prvek vkladat.
 * @param inserted ukazatel na vkladany prvek
 * @note V pripade prosteho pridavani na konec seznamu staci nastavit hodnotu elem na list->last.
 * @warning Touto funkci nelze pridavat prvni prvek seznamu
 * @warning Funkce nijak nekontroluje prislusnost prvku elem k seznamu list!!! Pri zadani prvku elem ktery neni soucasti seznamu list nastava nedefinovane chovani!
 */
void TAC_insert_post(TTacList *list, TThreeAC *elem, TThreeAC *inserted);

/**
 * @brief Funkce prida prvek do seznamu list na jeho konec (za prvek list->last)
 * @author Jan Beran
 * @param list Seznam, do ktereho se bude prvek vkladat
 * @param inserted vkladany prvek
 * @note Prazdny seznam nijak nevadi. Prvek se automaticky napoji jako prvni i posledni.
 */
void TAC_insert(TTacList *list, TThreeAC *inserted);

/**
 * //TODO doplnit doku
 * @param list
 * @param operand
 * @return
 */
TThreeAC *TAC_remove_this(TTacList *list, TThreeAC *operand);
/**
 * @brief Funkce maze posledni prvek ze seznamu (prvek oznaceny jako list->last) a vraci ho.
 * @author Jan Beran
 * @param list Seznam, ze ktereho ma byt prvek odstranen
 * @return Odstraneny prvek prostrednictvim ukazatele na strukturu TThreeAC nebo NULL pokud uz je prazdny
 */
TThreeAC *TAC_remove(TTacList *list);

/**
 * @brief Funkce kompletne smaze a dealokuje vsechny prvky seznamu list i seznam samotny, ktery nastavi na NULL.
 * @author Jan Beran
 * @param list Seznam ke smazani.
 * @warning NEVYUZITO - ponechano pro ukazku, ze to umime
 */
void TAC_delete_list(TTacList *list);

/**
 * @brief
 * @author Matej Jelinek
 * @param name - nazev instrukce
 * @param destination
 * @param op1
 * @param op2
 * @param collector
 * @return tf
 */
bool instruc_init(TTacList *list, int name, Toperand *destination, Toperand *op1, Toperand *op2, char *names[], Tgarbage_collector *collector);

/**
 * @brief do seznamu list vlozi triadresnou instrukci defvar s operandem op1
 * @author Matej Jelinek
 * @param list - do nej se prida ta instrukce
 * @param op1 - tenhle operand se tam prida
 * @return true pokud se vse povede (alokace apod.), jinak false
 */
bool tac_defvar(TTacList *list, Toperand *op1, Tgarbage_collector *collector);

bool tac_move(TTacList *list, Toperand *dest, Toperand *op1, Tgarbage_collector *collector);

bool tac_defmove_const( TTacList *list, Toperand *dest, Toperand *op1, Tgarbage_collector *collector);

bool tac_loadparam(TTacList *list, Toperand *dest, Tgarbage_collector *collector);

bool tac_push(TTacList *list, Toperand *op1, Tgarbage_collector *collector);

bool tac_add(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_sub(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_mul(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_div(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_call(TTacList *list, Toperand *dest, Toperand *op1, Tgarbage_collector *collector);

bool tac_return(TTacList *list, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_lable(TTacList *list, Toperand *op1, Tgarbage_collector *collector);

bool tac_deffunc(TTacList *list, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_jump(TTacList *list, Toperand *op1, Tgarbage_collector *collector);

bool tac_jumpifneq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_eq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_gt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_lt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_gteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_lteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_neq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Tgarbage_collector *collector);

bool tac_while(TTacList *list, Tgarbage_collector *collector);

bool tac_endwhile(TTacList *list, Tgarbage_collector *collector);

bool ta_startif(TTacList *list, Tgarbage_collector *collector);

bool ta_endif(TTacList *list, Tgarbage_collector *collector);

char *codegen_temp_id_generator(Tgarbage_collector *collector);



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
    DPRINT,
    DEFFUNC,
    EQ,
    GT,
    LT,
    GTEQ,
    LTEQ,
    NEQ,
    LOADPARAM,
    DEFMOVE,
    WHILE,
    ENDWHILE,
    LOADPARAM_DEF,
    ADD_DEF,
    JUMPIFEQ_DEF,
    GTEQ_DEF,
    STARTIF,
    ENDIF
};

#endif //IFJ2018_TAC_H