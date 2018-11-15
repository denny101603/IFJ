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
*	@file symtable.c
*	@author Daniel Bubenicek, kod hash tabulky prevzat a upraven z druheho projektu do predmetu IJC na FIT VUT, jehoz jsem autorem
*	@brief definice funkci pro tabulku symbolu (hash table)
 *	v1.1: stale neotestovano
 *
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "err_codes.h"

Tsymbol_table *symtab_init(unsigned long size)
{
    Tsymbol_table *tab = (Tsymbol_table *) malloc(sizeof(Tsymbol_table) + sizeof(struct symbol_table_item *) * size);
    if(tab == NULL) //pokud se nepovedlo alokovat, vracim null
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        return NULL;
    }
    tab->size = 0;
    tab->arr_size = size;
    for(unsigned long i = 0; i < size; i++)
        tab->ptr[i] = NULL; //vsude nastavim null, aby bylo poznat ze jsou prazdne

    return tab;
}

unsigned long symtab_get_arr_size(Tsymbol_table *t)
{
    return t->arr_size;
}

Tsymbol_table_item * symtab_edit_add(Tsymbol_table *t, char *key, const bool defined, const int token_type, const long int num_of_params)
{
    unsigned long index = symtab_hash_function(key) % symtab_get_arr_size(t);
    Tsymbol_table_item *item = symtab_find(t, key);
    if(item == NULL) //jeste neexistuje
    {
        item = (Tsymbol_table_item *) malloc(sizeof(Tsymbol_table_item)); //protoze jsem existujici zaznam nenasel, udelam novy
        if (item == NULL) //pokud se nepovedlo alokovat, vracim null
        {
            fprintf(stderr, MESSAGE_ALLOCATION);
            return NULL;
        }
        item->next = t->ptr[index]; //vkladam na zacatek seznamu, ulozim si tedy odkaz na dalsi
        t->ptr[index] = item; //napojim
        (t->size)++; //zvysim pocet zaznamu v tabulce
        item->key = key; //vlozeni klice do zaznamu
    }
    //nasleduje nastaveni/uprava hodnot ve strukture
    item->defined = defined;
    item->token_type = token_type;
    return item;

}

Tsymbol_table_item *symtab_find(Tsymbol_table *t, const char *key)
{
    unsigned long index = symtab_hash_function(key) % symtab_get_arr_size(t);
    Tsymbol_table_item *tmp;
    for(tmp = t->ptr[index]; tmp != NULL; tmp = tmp->next)
        if (!strcmp(tmp->key, key)) //pokud je slovo stejne, vracim zaznam
            return tmp; //vracim ukazatel na zaznam
    return NULL;
}

void symtab_clear(Tsymbol_table *t)
{
    for(unsigned long i = 0; (i < t->arr_size) && t->size != 0; i++) //pro cele pole nebo dokud neni size 0
    {
        Tsymbol_table_item *item;
        if((item = t->ptr[i]) != NULL)
        {
            while(item != NULL)
            {
                Tsymbol_table_item *tmp = item->next; //zaloha pristi
                free(item->key);
                free(item);
                (t->size)--;
                item=tmp;
            }
            t->ptr[i] = NULL; //nastavim null, aby bylo poznat, ze je prazdny
        }
    }
}

void symtab_free(Tsymbol_table *t)
{
    symtab_clear(t);
    free(t);
    t = NULL;
}

unsigned long symtab_get_size(Tsymbol_table *t)
{
    return t->size;
}

unsigned int symtab_hash_function(const char *str)
{
    unsigned int h=0;     // 32bit
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}