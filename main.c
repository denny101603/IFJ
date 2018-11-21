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
    TSymtables_stack *symtabs_bin = (TSymtables_stack *) malloc(sizeof(TSymtables_stack));
    TBuffer *tokens_backup = (TBuffer *) malloc(sizeof(TBuffer)); //buffer pro zalohu tokenu
    buffer_init(tokens_backup);
    TS_stack_init(symtabs_bin);



    printf("navrat SA: %i", startSA(tac_list, symtabs_bin, tokens_backup));
    GEN_start(tac_list);

    TAC_delete_list(tac_list);
    TS_stack_free(symtabs_bin);
    free(symtabs_bin);

    delete_buffer(tokens_backup);

    return 0;
}