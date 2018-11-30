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
typedef struct gc Tgarbage_collector;


#define INIT_SIZE 256 //inicializacni velikost pole

#define EOL '\n'
#define EMPTY -1
#define NUM_OF_KEYWORDS 9

/**
*	@brief dymamicky alokovane pole pro potreby nacitani ze vstupu, samo si hlida a pripadne realokuje velikost
 *	@author Daniel Bubenicek
 *	@warning k upravam promennych jsou vyuzity funkce, neupravovat rucne!
*/
typedef struct{
    char *array;
    int lenght; //delka pole
    int used; //pocet vyuzitych znaku
    int buffer; //nacteny znak z minule
    bool buffer_flag; //jestli cist z bufferu
    bool eol_flag; //jestli predchazel EOL
}Tarray;

/**
 * @brief Struktura slouzici pro trasport hodnoty a atributu tokenu mezi LA a SA
 * @author Jan Beran
 * @note atribut i samotny token jsou dynamicky alokovane
 * @warning Pro set a get hodnot z a do struktury pouzivat specialni funkce
 */
typedef struct{
    int type; //pole pro typ tokenu
    char *attribute; //pole pro atribut tokenu
   // int t_used; //skutecna delka typu
    //int a_used; //skutecna delka atributu
    //int a_len; //maximalni delka atributu, pri presahu nutno realokovat
}Ttoken;

/**
*	@brief funkce pomoci ktere pozada syntakticky analyzator o dalsi token
*	@author Daniel Bubenicek, Jan Beran
 *	@param arr pole pro pripravu tokenu
 *	@note pole arr je v rezii syntaktickeho analyzatoru a je jen jedno
*	@return Ttoken - ukazatel na token
*/
Ttoken *get_token(Tarray *arr, Tgarbage_collector *collector);

/**
*	@brief Inicializace dyn. alok. pole (Tarray)
*	@author Daniel Bubenicek
*	@param[in, out] arr – pole k inicializaci
*	@return SUCCES nebo ERR_INTERNAL pro neuspesnou alokaci
*/
int arr_init(Tarray *arr, Tgarbage_collector *gc);

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
int arr_set_buffer(Tarray *arr, int c);

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
 int arr_get_buffer(Tarray *arr);

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
 *	@note pri neuspesne alokaci dealokuje arr
*/
char *arr_get_value(Tarray *arr, Tgarbage_collector *collector);

/**
 * @brief Inicializace tokenu
 * @author Jan Beran
 * @param in token Odkaz na token k inicializaci.
 * @return SUCCESS
 */
int token_init(Ttoken *token);

/**
 * @brief Funkce nahraje do tokenu jeho typ
 * @author Jan Beran
 * @param token - cilovy token (odkaz)
 * @param arr zdrojove pole
 * @warning Maximalni delka typu je INIT_SIZE-1
 * @note typy nahravat pomoci preddefinovanych maker
 * @return SUCCESS nebo ERR_INTERNAL
 */
int token_set_type(Ttoken *token, int token_type);

/**
 * @brief Funkce nahraje do tokenu jeho atribut.
 * @author Jan Beran
 * @param token - cilovy token (odkaz)
 * @paragraph arr - zdrojove pole
 * @note velikost pole token.attribute je automaticky regulovana
 * @return SUCCESS nebo ERR_INTERNAL
 */
int token_set_attribute(Ttoken *token, Tarray *arr, Tgarbage_collector *collector);

/**
 *
 * @brief Uvolni token
 * @author Jan Beran
 * @param token - token k zabiti
 * @note Uvolnuje jak token, tak jeho atribut
 */
void token_free(Ttoken *token);

/**
 * @brief Zjisti typ znaku (male, velke pismeno, cislice, ostatni) (viz enum char_type)
 * @author Daniel Bubenicek
 * @param c znak
 * @return char_type podle typu predaneho znaku
 */
int type_of_char(const int c);

/**
 * @brief porovnava str s polem keywords
 * @author Daniel Bubenicek
 * @param str kontrolovany retezec
 * @return false pro neni keyword, state keywordu pokud je to keyword
 */
int is_keyword(const char *str);

/**
 * @brief
 * @author Daniel Bubenicek
 */
enum char_type{SMALL, CAPITAL, NUM, OTHER};

/**
 *  @brief Vycet vsech stavu, pouzitych v KA pro lexikalni analyzu.
*   @author Jan Beran, Daniel Bubenicek
 *  @warning Zadny ze stavu nemuze mit hodnotu -1, protoze hodnota -1 je pouzivana jako EMPTY u tokenu
 *  @warning Zadny ze stavu nesmi mit hodnotu ERR_INTERNAL
 *  @warning keywords musi zustat v rade za sebou a poradi musi souhlasit s polem keywords (a nebyt 0)
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
    BLOCK_COMMMENT_3, //TODO vyplati se to opravovat?
    ONE_LINE_COMMENT,
    ID_0,
    ID_1,
    ID_2,
    ID_FCE, //todo denny udelat ve skeneru (zatim vzdy vraci id_2)
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
    KEY_DEF,//prvni keyword
    KEY_DO,
    KEY_ELSE,
    KEY_END,
    KEY_IF,
    KEY_NOT,
    KEY_NIL,
    KEY_THEN,
    KEY_WHILE, //posledni keyword
    EXPRESSION, //pro potreby SAVu
    BOTTOM_TOKEN, //pro potreby SAVu
    ACTION_MENSITKO, //pro potreby SAVu
    ACTION_VETSITKO, //pro potreby SAVu
    NOBODY_KNOWS, //univerzalni typ
    UNI_NUMBER, //spolecny typ int float
    BOOLEAN //pro triadresnak
};

//********************************************************NASLEDUJI NEVYUZITY KOD*********************************************************************************/

/**
 * @brief Funkce vraci typ tokenu
 * @author Jan Beran
 * @param in token - zdrojovy token (odkaz)
 * @return tyo tokenu (int/enum)
 */
//int token_get_type(Ttoken *token);

/**
 * @brief NON IMPLEMENTED
 * @author Jan Beran
 * @param token  - zdrojovy token
 * @warning FUNKCE NENI IMPLEMENTOVANA
 * @return
 */
//char *token_get_attribute(Ttoken *token);


#endif //IFJ2018_FSM_H
