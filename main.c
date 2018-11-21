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
#include "code_gen.h"

int main() {

    //ahoj, já jsem carbik a posilam comit
    //Galantní Jelen
    //Berry was here!

    //z toho by se jeden posral(hlavně denny)
    printf("Hell, World!\n");
    TTacList *tac_list = TAC_init();
    printf("navrat SA: %i", startSA(tac_list));
    GEN_start(tac_list);
    TAC_delete_list(tac_list);
/*    Tarray arr;
    arr_init(&arr); //Pole znak
    get_token(&arr);
    arr_free(&arr);*/
    return 0;
}