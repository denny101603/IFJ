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

char arr[][100] = {"START","LEX_ERROR","IFJ_CODE_PREAM","OP_PLUS","OP_MINUS","OP_DIV","OP_MULT","OP_EQAL_0","OP_EQAL_1","OP_EQAL_2","OP_NOT_EQ_0","OP_NOT_EQ_1","OP_MORE_0","OP_MORE_1","OP_MORE_EQUAL","OP_LESS_0","OP_LESS_1","OP_LESS_EQUAL","LEFT_BRACKET","RIGHT_BRACKET","OP_COMMA","EOL_0","EOL_1","BLOCK_COMMENT_0","BLOCK_COMMENT_1","BLOCK_COMMENT_2","BLOCK_COMMMENT_3","ONE_LINE_COMMENT","ID_0","ID_1","ID_2","KEY_WORD","EOF_STATE","STRING_0","STRING_1","ESCAPE_0","ESCAPE_1","ESCAPE_2","NUMBER_0","NUMBER_1","INTEGER","FLOAT_0","FLOAT_1","FLOAT_2","FLOAT_EXP_0","FLOAT_EXP_1","FLOAT_EXP_2","KEY_DEF","KEY_DO","KEY_ELSE","KEY_END","KEY_IF","KEY_NOT","KEY_NIL","KEY_THEN","KEY_WHILE"};

char *stateToString(int state)
{
    if (state == 99)
    {
        return("ERR_INTERNAL");
    }
        return(arr[state]);
}

int main() {
    Tarray arr;
    arr_init(&arr); //Pole znaku
    unsigned int cnt = 0;
    while(1)
    {
        Ttoken token = get_token(&arr);
        printf("%u:typ: %s atribut: %s\n",cnt++, stateToString(token_get_type(&token)), token.attribute);
        token_free(&token);
        if(token_get_type(&token) == EOF_STATE)
            break;
    }    
    arr_free(&arr);
    return 0;
}
