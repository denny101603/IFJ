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
 *	v1.1: opraveny chyby v komentech
*/

#ifndef IFJ2018_FSM_H
#define IFJ2018_FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 128 //inicializacni velikost pole

#define EOL '\n'

/**
*	@brief dymamicky alokovane pole pro potreby nacitani ze vstupu, samo si hlida a pripadne realokuje velikost
 *	@author Daniel Bubenicek
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
 * @brief Struktura slouzici pro trasport hodnoty a atributu tokenu mezi LA a SA
 * @author Jan Beran
 * @note pro operatory je attribut shodny s typem (type = '*' => attribute = '*')
 * @warning Pro load a get hodnot z a do struktury pouzivat prosim specialni funkce
 * @warning v0.5 = funkce hotove, netestovane.
 */
typedef struct{
    char *type; //pole pro typ tokenu
    char *attribute; //pole pro atribut tokenu
    int t_used; //skutecna delka typu
    int a_used; //skutecna delka atributu
    int a_len; //maximalni delka atributu, pri presahu nutno realokovat
}Ttoken;

/**
*	@brief funkce pomoci ktere pozada syntakticky analyzator o dalsi token
*	@author Daniel Bubenicek, Jan Beran
 *	@param arr pole pro pripravu tokenu
 *	@note pole arr je v rezii syntaktickeho analyzatoru a je jen jedno
*	@return Ttoken - token typu Ttoken
*/
Ttoken get_token(Tarray arr);

/**
*	@brief Inicializace dyn. alok. pole (Tarray)
*	@author Daniel Bubenicek
*	@param[in, out] arr – pole k inicializaci
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou alokaci
*/
int arr_init(Tarray *arr);

/**
*	@brief Pridani znaku do pole arr (hlida presahnuti lenght a pripadne realokuje na vetsi)
*	@author Daniel Bubenicek
*	@param c - znak k pridani
*	@param[in, out] arr – cilove pole
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou realokaci
 *	@note v pripade neuspesne realokace uvolni pamet alokovanou pro pole
*/
int arr_add_char(Tarray *arr, char c);

/**
 * @brief Funkce prida do bufferu v poli arr znak c
 * &author Jan Beran
 * @param[in, out] arr - cilove pole
 * @param c
 * @return SUCCESS nebo ERR_INTERNAL, pokud arr == NULL
 */
int arr_add_to_buffer(Tarray *arr, char c);

/**
 *  @brief Funkce, pres kterou se nacita novy znak v KA Lex. analyzatoru. Pri flagu buffer_flag bere hodnotu z bufferu, jinak ze stdin
 * @author Jan Beran
 * @param arr - odkaz na pole pro tvorbu tokenu
 * @return nacteny znak
 */
int get_next_char(Tarray *arr);

/**
 * @brief Funkce vraci hodnotu c z pole arr a rovnou nastavuje flag buffer_flag na false
 * @author Jan Beran
 * @param arr - odkaz na pole typu Tarray
 * @return znak z bufferu
 * @note Jedna se o privatni funkci, pro ziskani znaku z bufferu doporucuji funkci get_next_char(), ktera rovnou hlida i flag
 * @warning Funkce se smi volat pouze pri buffer_flag = true
 */
 int get_from_buffer(Tarray *arr);
/**
*	@brief Nastavi used na 0, pripadne zmensi velikost alokovaneho pole na vychozi, aby setril misto
*	@author Daniel Bubenicek
*	@param[in, out] arr – cilove pole
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou realokaci
 *	@note v pripade neuspesne realokace uvolni pamet alokovanou pro pole
*/
int arr_reset(Tarray *arr);

/**
*	@brief Uvolni pamet pole
*	@author Daniel Bubenicek
*	@param[in, out] arr – cilove pole
*/
void arr_free(Tarray *arr);

/**
*	@brief vraci retezec s obsahem pole
*	@author Daniel Bubenicek
*	@param[in] arr – zdrojove pole
*	@return ukazatel na dyn. alokovany retezec zakonceny \0, pro prazdne pole "\0", pri neuspesne alokaci null
*/
char *arr_get_value(Tarray *arr);

/**
 * @brief Inicializace tokenu; dynamicka alokace pameti atp
 * @author Jan Beran
 * @param token - token k inicializace
 * @return SUCCESS nebo ERR_INTERNAL
 */
int token_init(Ttoken *token);

/**
 * @brief Funkce vraci retezec s polozkou token.type
 * @author Jan Beran
 * @param token - zdrojovy token
 * @return ukazatel na dyn. alokovany retezec s typem tokenu zakonceny '\0', pri prazdnem typu '\0, pri chybe NULL
 */
char *token_get_type(Ttoken *token);

/**
 * @brief Funkce vraci retezec s polozkou token.attribute
 * @author Jan Beran
 * @param token  - zdrojovy token
 * @return ukazatel na dyn. alokovany retezec s atributem tokenu zakonceny '\0', pri prazdnem typu '\0, pri chybe NULL
 */
char *token_get_attribute(Ttoken *token);

/**
 * @brief Funkce nahraje do tokenu jeho typ
 * @author Jan Beran
 * @param token - cilovy token
 * @param arr zdrojove pole
 * @warning Maximalni delka typu je INIT_SIZE-1
 * @note typy nahravat pomoci preddefinovanych maker
 * @return SUCCESS nebo ERR_INTERNAL
 */
int token_load_type(Ttoken *token, char *token_type);

/**
 * @brief Funkce nahraje do tokenu jeho atribut
 * @author Jan Beran
 * @param token - cilovy token
 * @paragraph arr - zdrojove pole
 * @note velikost pole token.attribute je automaticky regulovana
 * @return SUCCESS nebo ERR_INTERNAL
 */
int token_load_attribute(Ttoken *token, Tarray *arr);

/**
 *
 * @brief Uvolni token
 * @author Jan Beran
 * @param token - token k zabiti
 */
void token_free(Ttoken *token);

/**
 *  @brief Vycet vsech stavu, pouzitych v KA pro lexikalni analyzu.
*   @author Jan Beran
 */
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
    NUMBER_1,
    INTEGER,
    FLOAT_0,
    FLOAT_1,
    FLOAT_2,
    FLOAT_EXP_0,
    FLOAT_EXP_1,
    FLOAT_EXP_2,
};

#endif //IFJ2018_FSM_H
