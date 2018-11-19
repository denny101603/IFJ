
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
*	@file seman.c
*	@author Matěj Jelínek, Jan Beran
*	@brief soubor s definicemi funkci ze seman.h
 *	v1.2: Vsechny funkce vytvorene Janem Beranem otestovane v simulaci (nemam semanticky generator, ktery by je otestoval naostro).
 *	      V simulovanych testech vsechno fungovalo dobre.
*/
#include "seman.h"


TTacList *TAC_init()
{
    TTacList *new = malloc(sizeof(TTacList));
    if(new == NULL)
        return NULL; //err
    new->first = NULL;
    new->last = NULL;
}


void TAC_insert(TTacList *list, TThreeAC *inserted)
{
    if (list == NULL || inserted == NULL)//err
        return;
    //prazdny seznam
    if(list->first == NULL && list->last == NULL) // pokud se rovna NULL jen jeden, nekdo si s tim hral a je to chyba (neosetrovano, pri pouzivani fci se to nestane)
    {
       inserted->next = NULL;
       inserted->prev = NULL; //pro jistotu
       list->first = inserted;
       list->last = inserted;
    }
    else
    {
     inserted->next = NULL;
     list->last->next = inserted;
     inserted->prev = list->last;
     list->last = inserted;
    }
}

TThreeAC *TAC_remove(TTacList *list)
{
    if (list == NULL)
        return NULL;
    if(list->last == NULL && list->first == NULL) //zadny prvek. Pokud je NULL jen jeden => ERR
    {
        return NULL;
    }
    //alespon jeden prvek
    TThreeAC *ret = list->last;
    if(list->last == list->first) //jediny prvek => last se do NULL hodi sam, firstu je treba pomoci :)
        list->first = NULL;
    list->last = list->last->prev; // if list->last->prev == NULL, neni problem, stale OK
    if(list->last != NULL) //pokud NULL neni, korektne zaslepim
        list->last->next = NULL;
    ret->prev = NULL; //odpojeni od listu
    ret->next = NULL; //jistota, neni nutne
    return ret;
}


void TAC_insert_post(TTacList *list, TThreeAC *elem, TThreeAC *inserted)
{
    //neni seznam, neni urceno, za co vkladat, neni urceno co vkladat, seznam je prazdny -> ERR
    // (list->last == NULL netestovano, first a last jsou bud zaroven NULL nebo zaroven NULL nejsou pri pouziti fci) //todo denny, dokaz opak :)
    if(list == NULL || elem == NULL || inserted == NULL || list->first == NULL)
        return;

    inserted->next = elem->next;
    if(elem->next != NULL)
        elem->next->prev = inserted;
    elem->next = inserted;
    inserted->prev = elem;
    if(elem == list->last)
         list->last = inserted;

}

TThreeAC *TAC_remove_post(TTacList *list, TThreeAC *elem)
{
    //neexistujici seznam, prvek, prazdny seznam, jednoprvkovy seznam -> ERR
    if(list == NULL || elem == NULL || list->first == NULL || list->first->next == NULL)
        return  NULL;
    if(elem->next == NULL) //elem je posledni prvek listu, neni co odstranit. Tohle ERR neni, jen to je hovadina :D
        return NULL;
    TThreeAC *ret = NULL;
    ret = elem->next;
    if(elem->next->next == NULL) //odstranuji posledni prvek
    {
        list->last = elem;
        list->last->next = NULL;
        ret->next = NULL;
        ret->prev = NULL;
    }
    else //odstranuji prvek kdesi uprostred, musim napojit
    {
        elem->next = elem->next->next;
        elem->next->prev = elem;
        ret->next = NULL;
        ret->prev = NULL;
    }
    return ret;
}

void TAC_delete_list(TTacList *list)
{
    while(list->first != NULL)
    {
        /**Ladici vypisy, pred pouzitim zakomentovat**/
       /* TThreeAC *item = list->first;
        while(item != NULL)
        {
            printf("%d\n", item->name);
            item = item->next;
        }
        printf("_________________________\n");*/
        /**konec ladiciho vypisu**/
        free(TAC_remove(list));
    }

}
