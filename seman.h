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
*	@file seman.h
*	@author Matěj Jelínek, Jan Beran
*	@brief knihovna pro potreby konecneho automatu pro lexikalni analyzator
 *	v1.1: opraveny chyby v komentech
*/
#ifndef PROJECT_IFJCODE_H
#define PROJECT_IFJCODE_H

#include "fsm.h"
#include "symtable.h"
#include "sax.h"


/**
*	@brief struktura operandu
 *	@author Matej Jelinek
*/
typedef struct operand{
    bool variable_flag;
    int type;
    char *value;
}TOperand;

/**
 * @brief Struktura pro jednu triadresnou instrukci.
 * @authors Jan Beran, Matej Jelinek
 * @warning Buffer pouzivat jen pro funkce print a substr
 **/
typedef struct three_ac{
    int name;
    struct operand destination;
    struct operand op_1;
    struct operand op_2;
    struct TAC_buffer; //Todo udelat buffer pro parametry fci print a substr
    struct three_ac *next;
    struct three_ac *prev;
}TThreeAC;


/**
 * @brief Struktura pro seznam triadresnych instrukci, kde instrukce na odkaze oznacenem jako "first" je prvni instrukci programu.
 * @author Jan Beran
 */
typedef struct tac_list{
    TThreeAC *first;
    TThreeAC *last;
}TTacList;

/**
 * @brief Funkce pro vytvoreni a alokaci seznamu triadresnych instrukci (Konstruktor).
 * @author Jan Beran
 * @return Ukazatel na nove vytvoreny seznam typu TTacList
 */
TTacList *TAC_init();


/**
 * @brief Funkce vlozi prvek inserted na misto ZA prvek elem v seznamu list.
 * @author Jan Beran
 * @param elem ukazatel na prvek seznamu list, ZA ktery ma byt vlozen prvek inserted.
 * @param list ukazatel na seznam, do ktereho se bude prvek vkladat.
 * @param inserted ukazatel na vkladany prvek
 * @note V pripade prosteho pridavani na konec seznamu staci nastavit hodnotu elem na list->last.
 */
void TAC_insert_post(TTacList *list, TThreeAC *elem, TThreeAC *inserted);


/**
 * @brief Funkce odstrani prvek ZA prvek elem ze seznamu list a vrati ho.
 * @author Jan Beran
 * @param list ukazatel na seznam
 * @param elem ukazatel na prvek, ZA kterým se nachází mazaný prvek
 * @return smazany prvek.
 * @note Pro mazani posledniho prvku staci parametr elem nastavit na list->last->prev.
 * @warning Touto funkci nelze mazat prvni prvek seznamu!
 */
TThreeAC *TAC_remove_post(TTacList *list, TThreeAC *elem);

/**
 * @brief Funkce kompletne smaze a dealokuje vsechny prvky seznamu list i seznam samotny, ktery nastavi na NULL.
 * @author Jan Beran
 * @param list Seznam ke smazani.
 */
void TAC_delete_list(TTacList *list);


/**
 * @author Matej Jelinek
 * @param instruct
 * @return
 */
bool ns_defvar(TThreeAC *instruct);

#endif //IFJ2018_SEMAN_H
