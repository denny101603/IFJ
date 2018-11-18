/*******************
* IFJ projekt 2018
*
* Doplnim popis
*
* Jan Beran xberan43
* Daniel Bubenicek xbuben05
* Jan Carba xcarba00
* Matej Jelinek xjelin49
*******************************/
#include <stdio.h>
#include "fsm.h"
#include "sax.h"
#include "seman.h"
int main() {

    //ahoj, já jsem carbik a posilam comit
    //Galantní Jelen
    //Berry was here!
    /**smazat odsud**/
    TTacList *list = TAC_init();
    TThreeAC *item = NULL;
    for(int i = 0; i < 10; i++)
    {
        item = malloc(sizeof(TThreeAC));
        if(item == NULL)
            return -1;
        item->name = i;
        TAC_insert(list, item);
    }
    item = list->first;
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (naplneni)____________________________\n");
    item = list->first->next->next->next; //item->name == 3
    TThreeAC *inserted = malloc(sizeof(TThreeAC));
    inserted->name = 35;
    TAC_insert_post(list, item, inserted);
    item = list->first;
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (insert post doprostred)____________________________\n");
    item = list->first->next->next->next; //item->name == 3
    TAC_remove_post(list, item);
    item = list->first;
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (remove post doprostred)____________________________\n");
    item = list->first;
    inserted = TAC_remove(list);
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (remove)____________________________\n");
    item = list->first;
    TAC_insert(list, inserted);
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (insert)____________________________\n");
    inserted = TAC_remove_post(list, list->last->prev);
    item = list->first;
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (remove post v rezimu remove last)____________________________\n");
    TAC_insert_post(list, list->last, inserted);
    item = list->first;
    while(item != NULL)
    {
        printf("%d\n", item->name);
        item = item->next;
    }
    printf("_Konec testu 1 (insert post v rezimu insert last)____________________________\n");
    TAC_delete_list(list);
    return 0;
    /** smazat po sem**/
    //z toho by se jeden posral(hlavně denny)
    printf("Hell, World!\n");
    printf("navrat SA: %i", startSA());
/*    Tarray arr;
    arr_init(&arr); //Pole znaku
    get_token(&arr);
    arr_free(&arr);*/
    return 0;
}