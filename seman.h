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
*	@brief knihovna pro potreby semantickeho generatoru
 *	v1.2: Vsechny funkce vytvorene Janem Beranem otestovane v simulaci (nemam semanticky generator, ktery by je otestoval naostro).
 *	      V simulovanych testech vsechno fungovalo dobre.
*/
#ifndef PROJECT_IFJCODE_H
#define PROJECT_IFJCODE_H

#include <stdlib.h>
#include <stdbool.h>

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
    Ttac_buffer *buffer;
    struct three_ac *next;
    struct three_ac *prev;
}TThreeAC;

//todo berry udelat tyhle funkce
/**
 *
 * @return
 */
Ttac_buffer *op_buffer_init();

void op_push(Ttac_buffer *buffer, Toperand *operand);

Toperand *op_pop(Ttac_buffer *buffer);

void op_delete_buffer(Ttac_buffer *buffer);


/**
 * @brief alokuje strukturu Toperand a naplni ji
 * @author Daniel Bubenicek
 * @param type typ operandu
 * @param name jmeno promenne nebo u konstant hodnota
 * @return operand, pokud se nepovede alokace tak NULL
 */
Toperand *op_init(int type, char *name);


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
 * @return Ukazatel na nove vytvoreny seznam typu TTacList nebo NULL pri neuspesne alokaci
 */
TTacList *TAC_init();


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
 * @brief Funkce maze posledni prvek ze seznamu (prvek oznaceny jako list->last) a vraci ho.
 * @author Jan Beran
 * @param list Seznam, ze ktereho ma byt prvek odstranen
 * @return Odstraneny prvek prostrednictvim ukazatele na strukturu TThreeAC
 */
TThreeAC *TAC_remove(TTacList *list);

/**
 * @brief Funkce kompletne smaze a dealokuje vsechny prvky seznamu list i seznam samotny, ktery nastavi na NULL. //TODO BERRY! by denny seznam jako takovy nedealokujes, naprav to! :D
 * @author Jan Beran
 * @param list Seznam ke smazani.
 */
void TAC_delete_list(TTacList *list);


/**
 * @brief
 * @author Matej Jelinek
 * @param name - nazev instrukce
 * @param destination
 * @param op1
 * @param op2
 * @return tf
 */
bool instruc_init(TTacList *list, int name, Toperand *destination, Toperand *op1, Toperand *op2);

/**
 * @brief do seznamu list vlozi triadresnou instrukci defvar s operandem op1
 * @author Matej Jelinek
 * @param list - do nej se prida ta instrukce
 * @param op1 - tenhle operand se tam prida
 * @return true pokud se vse povede (alokace apod.), jinak false
 */
        bool tac_defvar(TTacList *list, Toperand *op1);

        bool tac_move(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_defmove_const( TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_createframe(TTacList *list);

        bool tac_pushframe(TTacList *list);

        bool tac_popframe(TTacList *list);

        bool tac_loadparam(TTacList *list, Toperand *dest);

        bool tac_push(TTacList *list, Toperand *op1);

        bool tac_pop(TTacList *list, Toperand *dest);

        bool tac_add(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_sub(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_mul(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_div(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_inputi(TTacList *list, Toperand *dest);

        bool tac_inputs(TTacList *list, Toperand *dest);

        bool tac_inputf(TTacList *list, Toperand *dest);

        bool tac_print(TTacList *list, Ttac_buffer *buffer);

        bool tac_length(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_substr(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2, Ttac_buffer *buffer);

        bool tac_ord(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_chr(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_call(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_return(TTacList *list, Toperand *op1);

        bool tac_int2float(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_float2int(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_int2char(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_concat(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_setchar(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_isint(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_isfloat(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_isstring(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_isbool(TTacList *list, Toperand *dest, Toperand *op1);

        bool tac_lable(TTacList *list, Toperand *op1);

        bool tac_deffunc(TTacList *list, Toperand *op1);

        bool tac_jump(TTacList *list, Toperand *op1);

        bool tac_jumpifeq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_jumpifneq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_jumpifgt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_jumpiflt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_dprint(TTacList *list, Toperand *op1);

        bool tac_eq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_gt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_lt(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_gteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_lteq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);

        bool tac_neq(TTacList *list, Toperand *dest, Toperand *op1, Toperand *op2);




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
    LOADPARAM
};



#endif //IFJ2018_SEMAN_H
