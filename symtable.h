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
*	@file symtable.h
*	@author Daniel Bubenicek, kod hash tabulky prevzat a upraven z druheho projektu do predmetu IJC na FIT VUT, jehoz jsem autorem
*	@brief knihovna pro tabulku symbolu (hash table)
 *	v1.0:
 *
*/

#ifndef IFJ2018_SYMTABLE_H
#define IFJ2018_SYMTABLE_H
#include <stdbool.h>

typedef struct symbol_table_item
{
    char *key; //nazev promenne !!dynamicky alokovano!!
    bool defined; //0 jeste nebyla definovana, 1 uz ano
    int token_type; //typ z tokenu
    struct symbol_table_item *next;
}Tsymbol_table_item;

typedef struct symbol_table
{
    unsigned long size; //aktualni pocet zaznamu
    unsigned long arr_size; //velikost pole ukazatelu
    struct symbol_table_item *ptr[];
}Tsymbol_table;


/**
*	@brief inicializace hash tabulky(TS) - dynamicky alokuje misto pro Tsymbol_table i s polem
*	@author Daniel Bubenicek
 *	@param size - velikost pole pro itemy
*	@return NULL pro neuspesnou alokaci jinak ukazatel na TS
*/
Tsymbol_table *symtab_init(unsigned long size);

/**
*	@brief vraci delku pole hash tabulky
*	@author Daniel Bubenicek
 *	@param t - tabulka jejiz velikost chci znat
*	@return delku pole
*/
unsigned long symtab_get_arr_size(Tsymbol_table *t);

/**
*	@brief upravi nebo prida item v tabulce symbolu
*	@author Daniel Bubenicek
*	@param[in, out] t - tabulka symbolu se kterou bude pracovat
*	@param key – vyhledavany nazev (ukazatel na DA pamet), v pripade zalozeni noveho itemu ulozi odkaz na nej do strukury
 *	@param defined - pro upravu/inicializaci tehoz ve strukture
 *	@param token_type - pro upravu/inicializaci tehoz ve strukture
*	@return NULL pro neuspesnou alokaci jinak ukazatel na dany item (mozne dalsi upravy)
*/
Tsymbol_table_item *symtab_edit_add(Tsymbol_table *t, char *key, const bool defined, const int token_type);

/**
*	@brief najde a vrati item v tabulce symbolu
*	@author Daniel Bubenicek
*	@param[in, out] t - tabulka symbolu se kterou bude pracovat
*	@param key – vyhledavany nazev
*	@return NULL pro neuspesnou nenalezeni jinak ukazatel na dany item (mozne dalsi upravy)
*/
Tsymbol_table_item *symtab_find(Tsymbol_table *t, const char *key);

/**
*	@brief zruseni vsech itemu, tabulka zustane prazdna
*	@author Daniel Bubenicek
*	@param[in, out] t - rusena tabulka
*/
void symtab_clear(Tsymbol_table *t);

/**
*	@brief dealokuje celou tabulku
*	@author Daniel Bubenicek
*	@param[in, out] t - rusena tabulka
 *	@note vola fci symtab_clear
*/
void symtab_free(Tsymbol_table *t);

unsigned int symtab_hash_function(const char *str);

#endif //IFJ2018_SYMTABLE_H
