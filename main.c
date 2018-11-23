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

char *enum2string(int num){
    enum {DEFVAR,MOVE,CREATEFRAME,PUSHFRAME,POPFRAME,PUSH,POP,ADD,SUB,MUL,DIV,INPUTI,INPUTS,INPUTF,PRINT,LENGTH,SUBSTR,ORD,CHR,CALL,
        RETURN,INT2FLOAT,FLOAT2INT,INT2CHAR,CONCAT,SETCHAR,ISINT,ISFLOAT,ISSTRING,ISBOOL,LABLE,JUMP,JUMPIFEQ,JUMPIFNEQ,JUMPIFGT,JUMPIFLT,DPRINT,
        DEFFUNC,EQ,GT,LT,GTEQ,LTEQ,NEQ,LOADPARAM,DEFMOVE,WHILE,ENDWHILE};
    switch (num)
    {
        case DEFVAR:
            return "DEFVAR";
            break;
        case MOVE:
            return "MOVE";
            break;
        case CREATEFRAME:
            return "CREATEFRAME";
            break;
        case PUSHFRAME:
            return "PUSHFRAME";
            break;
        case POPFRAME:
            return "POPFRAME";
            break;
        case PUSH:
            return "PUSH";
            break;
        case POP:
            return "POP";
            break;
        case ADD:
            return "ADD";
            break;
        case SUB:
            return "SUB";
            break;
        case MUL:
            return "MUL";
            break;
        case DIV:
            return "DIV";
            break;
        case INPUTI:
            return "INPUTI";
            break;
        case INPUTS:
            return "INPUTS";
            break;
        case INPUTF:
            return "INPUTF";
            break;
        case PRINT:
            return "PRINT";
            break;
        case LENGTH:
            return "LENGTH";
            break;
        case SUBSTR:
            return "SUBSTR";
            break;
        case ORD:
            return "ORD";
            break;
        case CHR:
            return "CHR";
            break;
        case CALL:
            return "CALL";
            break;
        case RETURN:
            return "RETURN";
            break;
        case INT2FLOAT:
            return "INT2FLOAT";
            break;
        case FLOAT2INT:
            return "FLOAT2INT";
            break;
        case INT2CHAR:
            return "INT2CHAR";
            break;
        case CONCAT:
            return "CONCAT";
            break;
        case SETCHAR:
            return "SETCHAR";
            break;
        case ISINT:
            return "ISINT";
            break;
        case ISFLOAT:
            return "ISFLOAT";
            break;
        case ISSTRING:
            return "ISSTRING";
            break;
        case ISBOOL:
            return "ISBOOL";
            break;
        case LABLE:
            return "LABLE";
            break;
        case JUMP:
            return "JUMP";
            break;
        case JUMPIFEQ:
            return "JUMPIFEQ";
            break;
        case JUMPIFNEQ:
            return "JUMPIFNEQ";
            break;
        case JUMPIFGT:
            return "JUMPIFGT";
            break;
        case JUMPIFLT:
            return "JUMPIFLT";
        case DPRINT:
            return "DPRINT";
        case DEFFUNC:
            return "DEFFUNC";
            break;
        case EQ:
            return "EQ";
            break;
        case GT:
            return "GT";
            break;
        case LT:
            return "LT";
            break;
        case GTEQ:
            return "GTEQ";
            break;
        case LTEQ:
            return "LTEQ";
            break;
        case NEQ:
            return "NEQ";
            break;
        case LOADPARAM:
            return "LOADPARAM";
            break;
        case DEFMOVE:
            return "DEFMOVE";
            break;
        case WHILE:
            return "WHILE";
            break;
        case ENDWHILE:
            return "ENDWHILE";

    }

}

int main() {



    //ahoj, já jsem carbik a posilam comit
    //Galantní Jelen
    //Berry was here!

    //z toho by se jeden posral(hlavně denny)
    //printf("Hell, World!\n");
    TTacList *tac_list = TAC_init();
    TSymtables_stack *symtabs_bin = (TSymtables_stack *) malloc(sizeof(TSymtables_stack));
    TBuffer *tokens_backup = (TBuffer *) malloc(sizeof(TBuffer)); //buffer pro zalohu tokenu
    buffer_init(tokens_backup);
    TS_stack_init(symtabs_bin);

    int i = startSA(tac_list, symtabs_bin, tokens_backup);
    //odmazat pod
/*    TThreeAC *tt = tac_list->first;
    while(tt != NULL)
    {
        printf("%i\n", tt->name);
        tt = tt->next;
    }*/
    //odmazat nad
    /*printf("navrat SA: %i", i);*/
    GEN_start(tac_list);
/*
    TAC_delete_list(tac_list);
    TS_stack_free(symtabs_bin);
    free(symtabs_bin);

    delete_buffer(tokens_backup);
*/
    return i;
}