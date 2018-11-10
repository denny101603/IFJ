//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAX_H
#define IFJ2018_SAX_H

#endif //IFJ2018_SAX_H

#include "savo.h"
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
 * @brief Struktura, slouzici pro komunikaci mezi scannerem, savem a saxem.
 * @param arr Dennyho megahustodemonskykrutoprisne pole, ktere vsechny jenom sere :)
 * @param buffer buffer (vyrovnavaci pamet) pro komunikaci mezi sax a savo
 */
typedef struct SynCommon{
    Tarray arr;
    TBuffer buffer;
   // TSYmTable table_fun;
   //TSymTable table_local;
} TSynCommon;
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
bool buffer_push(TBuffer *buffer, Ttoken *token);

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

/**
 * @brief Funkce vraci dalsi token a na zaklade toho, zda se v bufferu nachazi nejake tokeny, je bere bud z nej, nebo ze stdin pomoci scanneru.
 * @author Jan Beran, Daniel Bubenicek (arr)
 * @param arr Dennyho megahustodemonskykrutoprisne pole, ktere vsechny jenom sere :)
 * @param buffer buffer pro uchovavani tokenu
 * @return Token z prislusneho zdroje
 */
Ttoken get_next_token(Tarray *arr, TBuffer *buffer);

