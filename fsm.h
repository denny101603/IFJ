/***************************************
* 	IFJ projekt 2018                   *
* 						               *
*	Autori:			                   *
*	Jan Beran (xberan43)	           *
*	Daniel Bubenicek (xbuben43)	       *
*	Jan Carba (xcarba00)		       *
*	Matej Jelinek (xjelen49)	       *
*                                      *
***************************************/
/**
*	@file fsm.h
*	@author Daniel Bubenicek, Jan Beran
*	@brief knihovna pro potreby konecneho automatu pro lexikalni analyzator
*/

#ifndef IFJ2018_FSM_H
#define IFJ2018_FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 128 //inicializacni velikost pole

/**
*	@brief dymamicky alokovane pole pro potreby nacitani ze vstupu, samo si hlida a pripadne realokuje velikost
 *	@warning k upravam promennych array, lenght, used jsou vyuzity funkce, neupravovat rucne!
*/
typedef struct{
    char *array;
    int lenght; //delka pole
    int used; //pocet vyuzitych znaku
    char buffer; //nacteny znak z minule
    bool buffer_flag; //jestli cist z bufferu
    bool eol_flag; //jestli predchazel EOL
}Tarray;


/**
*	@brief funkce pomoci ktere pozada syntakticky analyzator o dalsi token
*	@author Daniel Bubenicek
*	@param jmeno_parametru – hodnoty
*	@param[in, out] jmeno_ukazatele – [in, out] = vstupni, vystupni. Muze byt i jen [in] nebo [out]
*	@return navratova_hodnota, pokud existuje
*	@note poznamka (nepovinne)
*/
int get_token();

/**
*	@brief Inicializace dyn. alok. pole (Tarray)
*	@author Daniel Bubenicek
*	@param[in, out] arr – pole k inicializaci
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou alokaci
*/
int arr_init(Tarray *arr);

/**
*	@brief Pridani znaku do pole arr (hlida presahnuti lenght a pripadne realukuje na vetsi)
*	@author Daniel Bubenicek
*	@param c - znak k pridani
*	@param[in, out] arr – cilove pole
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou realokaci
 *	@note v pripade neuspesne realokace uvolni pamet alokovanou pro pole
*/
int arr_add_char(Tarray *arr, char c);

/**
*	@brief nastavi used na 0, pripadne zmensi velikost alokovaneho pole na vychozi, aby setril misto
*	@author Daniel Bubenicek
*	@param[in, out] arr – cilove pole
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou realokaci
 *	@note v pripade neuspesne realokace uvolni pamet alokovanou pro pole
*/
int arr_reset(Tarray *arr);

/**
*	@brief uvolni pamet pole
*	@author Daniel Bubenicek
*	@param[in, out] arr – cilove pole
*/
void arr_free(Tarray *arr);

/**
*	@brief vraci retezec s obsahem pole
*	@author Daniel Bubenicek
*	@param[in] arr – cilove pole
*	@return ukazatel na dyn. alokovany retezec zakonceny \0, pro prazdne pole "\0", pri neuspesne alokaci null
*/
char *arr_get_value(Tarray *arr);


enum states {
    START,
    LEX_ERROR,
    IFJ_CODE_PREAM, //JAKOZE PREAMBULE :)
    OP_PLUS,
    OP_MINUS,
    OP_DIV,
    OP_MULT,
    OP_EQAL_0,
    OP_EQAL_1,
    OP_EQAL_2,
    OP_NOT_EQ_0,
    OP_NOT_EQ_1,
    OP_MORE_0,
    OP_MORE_1,
    OP_MORE_EQUAL,
    OP_LESS_0,
    OP_LESS_1,
    OP_LESS_EQUAL,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    OP_COMMA,
    EOL_0,
    EOL_1,
    BLOCK_COMMENT_0,
    BLOCK_COMMENT_1,
    BLOCK_COMMENT_2,
    BLOCK_COMMMENT_3,
    ONE_LINE_COMMENT,
    ID_0,
    ID_1,
    ID_2,
    KEY_WORD,
    EOF_STATE,
    STRING_0,
    STRING_1,
    ESCAPE_0,
    ESCAPE_1,
    ESCAPE_2,
    NUMBER_0,
    INTEGER,
    FLOAT_0,
    FLOAT_1,
    FLOAT_2,
    FLOAT_EXP_0,
    FLOAT_EXP_1,
    FLOAT_EXP_2,
};

#endif //IFJ2018_FSM_H
