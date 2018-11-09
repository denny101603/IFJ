//
// Created by janbe on 08.11.2018.
//

#ifndef IFJ2018_SAVO_H
#define IFJ2018_SAVO_H

#endif //IFJ2018_SAVO_H
#include "fsm.h"
#include "fsm.c"

char prec_table[15][15] = {//TODO
       /* + */ {'>','>','<','<','<','>','<','?','>','>','>','>','>','>','>'},
       /* - */ {'>','>','<','<','<','>','<','?','>','>','>','>','>','>','>'},
       /* * */ {'>','>','>','>','<','>','<','?','>','>','>','>','>','>','>'},
       /* / */ {'>','>','>','>','<','>','<','?','>','>','>','>','>','>','>'},
       /* ( */ {'<','<','<','<','<','=','<','?','<','<','<','<','<','<','<'},
       /* ) */ {},
       /* id */{},
       /* cons */ {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}
};
//todo
typedef struct{

}TStack;
//todo
char get_action(Ttoken input_token, Ttoken stack_token);
bool is_end_of_expression(Ttoken token);

//todo
Ttoken get_first_terminal(TStack stack);
void push(TStack stack, Ttoken token);
void pop(TStack stack, Ttoken token);