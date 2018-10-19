/*******************
* IFJ projekt 2018
*
* TESTOVANI SCANNERU - by denny
*
* Jan Beran xberan43
* Daniel Bubenicek xbuben05
* Jan Carba xcarba00
* Matej Jelinek xjelin49
*******************************/
#include <stdio.h>
#include "fsm.h"
#include "err_codes.h"

int main() {
    Tarray arr;
    arr_init(&arr); //Pole znaku
    unsigned int cnt = 0;
    while(1)
    {
        Ttoken token = get_token(&arr);
        printf("%u:typ: %i atribut: %s\n",cnt++, token_get_type(&token), token_get_attribute(&token));
        token_free(&token);
        if(token_get_type(&token) == EOF_STATE)
            break;
    }    
    arr_free(&arr);
    return 0;
}
