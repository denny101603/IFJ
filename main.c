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
#include "garbage_collector.h"
#include "scanner.h"
#include "parser.h"
#include "seman.h"
#include "code_gen.h"
#include "err_codes.h"

int main() {

    Tgarbage_collector *collector = malloc(sizeof(collector)); //dealokovan samostatne ve valaru
    collector->last_added = NULL;

    TTacList *tac_list = TAC_init(collector);
    TSymtables_stack *symtabs_bin = (TSymtables_stack *) malloc(sizeof(TSymtables_stack)); //symtables jedou na svoje triko
    gc_add_garbage(collector,symtabs_bin );

    TS_stack_init(symtabs_bin);

    int i = startSA(tac_list, symtabs_bin, collector);

    if(i == SUCCESS) //== 0
        GEN_start(tac_list, collector);

    TS_stack_free(symtabs_bin);

    gc_dealloc_all(collector);
    return i;
}