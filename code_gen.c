

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
*	@file code_gen.c
*	@author Matej Jelinek, Jan Carba
*	@brief soubor s definicemi funkci ze code_gen.h
*/

#include "code_gen.h"
#include "seman.h"
#include "fsm.h"
#include <stdio.h>
#include <stdbool.h>

char *codegen_temp_id_generator()
{
    static unsigned long long cnt = 0;
    char *name = (char *) malloc(sizeof(char)*32);
    if(name == NULL)
        return NULL;
    sprintf(name, "&codegen%llu", cnt++);
    return name;
}

void pream()
{
    //pream
    printf(".IFJcode18\n");
    printf("JUMP main\n");

    //inputi
    printf("LABEL inputi\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@var\n");
    printf("READ LF@var int\n");
    printf("PUSHS LF@var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //inputf
    printf("LABEL inputf\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@var\n");
    printf("READ LF@var float\n");
    printf("PUSHS LF@var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //inputs
    printf("LABEL inputi\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@var\n");
    printf("READ LF@var string\n");
    printf("PUSHS LF@var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //length
    printf("LABEL length\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@var\n");
    printf("DEFVAR LF@x\n");
    printf("POPS LF@x");
    printf("TYPE LF@xtype LF@x\n");
    printf("JUMPIFNEQ lengtherror LF@xtype string@string\n");
    printf("STRLEN LF@var LF@x\n");
    printf("SUB LF@var LF@var int@1\n");
    printf("PUSHS LF@var\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL lengtherror\n");
    printf("EXIT int@4\n");

    //print
    printf("LABEL length\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@counter\n");
    printf("DEFVAR LF@test\n");
    printf("DEFVAR LF@term\n");
    printf("POPS LF@counter\n");
    printf("LABEL loop\n");
    printf("POPS LF@term\n");
    printf("WRITE LF@term\n");
    printf("SUB LF@counter LF@counter int@0\n");
    printf("GT LF@test LF@counter int@0\n");
    printf("JUMPIFEQ loop LF@test bool@true\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //začátek kódu
    printf("LABEL main\n");

}

void gen_defvar(TThreeAC *instruct)
{
    printf("DEFVAR LF@%s\n", instruct->op_1->name);
}

void gen_move(TThreeAC *instruct)
{
    printf("MOVE LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name);
}

void gen_defmove_const(TThreeAC *instruct)
{

}

void gen_createframe(TThreeAC *instruct)
{
    printf("CREATEFRAME\n");
}

void gen_pushframe(TThreeAC *instruct)
{
    printf("PUSHFRAME\n");
}

void gen_popframe(TThreeAC *instruct)
{
    printf("POPFRAME\n");
}

void gen_loadparam(TThreeAC *instruct)
{
    printf("DEFVAR LF@%s\n"
           "POPS LF@%s\n", instruct->destination->name, instruct->destination->name);
}

void gen_push(TThreeAC *instruct)
{
    printf("PUSHS LF@%s\n", instruct->op_1->name);
}

void gen_pop(TThreeAC *instruct)
{
    printf("POPS LF@%s\n", instruct->destination->name);
}

void gen_add(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp

    char *addastring = codegen_temp_id_generator(); //label skoku když a je string
    char *addaint = codegen_temp_id_generator(); //label skoku když a je int
    char *addafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *addaintaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *addafloatbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *addend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *adderrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4
    char *addasametypestring = codegen_temp_id_generator(); //label skoku když je a i b string a chceme konkatenovat

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",addastring, atype);

    printf("JUMPIFEQ %s LF@%s string@int\n",addaint, atype);

    printf("JUMPIFEQ %s LF@%s string@float\n",addafloat , atype);

    printf("LABEL %s\n", addastring);
        printf("JUMPIFEQ %s LF@%s string@string\n", addasametypestring, btype);
        printf("JUMP %s\n", adderrorend);

    printf("LABEL %s\n", addaint);
        printf("JUMPIFEQ %s LF@%s string@float\n", addaintaretype, btype);
        printf("JUMPIFNEQ %s LF@%s string@int\n", adderrorend, btype);

        printf("ADD LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n", addend); //skočí na úspěšný konec add

        printf("LABEL %s\n", addaintaretype);
            printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
            printf("ADD LF@%s LF@%s LF@%s\n", instruct->destination->name, atmp, instruct->op_2->name);
            printf("JUMP %s\n", addend); //skočí na úspěšný konec add

    printf("LABEL %s\n", addafloat);
        printf("JUMPIFEQ %s LF@%s string@int\n", addafloatbretype, btype);
        printf("JUMPIFNEQ %s LF@%s string@float\n", adderrorend, btype);

        printf("ADD LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n", addend); //skočí na úspěšný konec add

        printf("LABEL %s\n", addafloatbretype);
            printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
            printf("ADD LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, btmp);
            printf("JUMP %s\n", addend); //skočí na úspěšný konec add

    printf("LABEL %s\n", addasametypestring);
        printf("CONCAT LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n", addend); //skočí na úspěšný konec add

    printf("EXIT int@4\n");

    printf("LABEL %s\n", addend);
}

void gen_sub(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp

    char *subaint = codegen_temp_id_generator(); //label skoku když a je int
    char *subafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *subaintaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *subafloatbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *subend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *suberrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@int\n",subaint, atype);

    printf("JUMPIFEQ %s LF@%s string@float\n",subafloat , atype);

    printf("LABEL %s\n", subaint);
        printf("JUMPIFEQ %s LF@%s string@float\n", subaintaretype, btype);
        printf("JUMPIFNEQ %s LF@%s string@int\n", suberrorend, btype);

        printf("SUB LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n", subend); //skočí na úspěšný konec sub

        printf("LABEL %s\n", subaintaretype);
            printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
            printf("SUB LF@%s LF@%s LF@%s\n", instruct->destination->name, atmp, instruct->op_2->name);
            printf("JUMP %s\n", subend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", subafloat);
        printf("JUMPIFEQ %s LF@%s string@int\n", subafloatbretype, btype);
        printf("JUMPIFNEQ %s LF@%s string@float\n", suberrorend, btype);

        printf("SUB LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n", subend); //skočí na úspěšný konec sub

        printf("LABEL %s\n", subafloatbretype);
            printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
            printf("SUB LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, btmp);
            printf("JUMP %s\n", subend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", suberrorend);
        printf("EXIT int@4\n");

    printf("LABEL %s", subend);
}

void gen_mul(TThreeAC *instruct)
{

}

void gen_div(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp

    char *divaint = codegen_temp_id_generator(); //label skoku když a je int
    char *divafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *divaintaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *divafloatbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *divend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *diverrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@int\n",divaint, atype);

    printf("JUMPIFEQ %s LF@%s string@float\n",divafloat , atype);

    printf("LABEL %s\n", divaint);
    printf("JUMPIFEQ %s LF@%s string@float\n", divaintaretype, btype);
    printf("JUMPIFNEQ %s LF@%s string@int\n", diverrorend, btype);

    printf("IDIV LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n", divend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", divaintaretype);
    printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
    printf("DIV LF@%s LF@%s LF@%s\n", instruct->destination->name, atmp, instruct->op_2->name);
    printf("JUMP %s\n", divend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", divafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n", divafloatbretype, btype);
    printf("JUMPIFNEQ %s LF@%s string@float\n", diverrorend, btype);

    printf("DIV LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n", divend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", divafloatbretype);
    printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
    printf("DIV LF@%s LF@%s LF@%s\n",instruct->destination->name, instruct->op_1->name, btmp);
    printf("JUMP %s\n", divend); //skočí na úspěšný konec sub

    printf("LABEL %s\n", diverrorend);
    printf("EXIT int@4\n");

    printf("LABEL %s", divend);

}

void gen_call(TThreeAC *instruct)
{
    printf("CALL %s\n", instruct->op_1->name);
    printf("POPS LF@%s\n", instruct->destination->name);
}

void gen_return(TThreeAC *instruct)
{
    printf("PUSHS LF@%s\n", instruct->op_1->name);
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_int2float(TThreeAC *instruct)
{
    printf("INT2FLOAT LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name);
}

void gen_float2int(TThreeAC *instruct)
{
    printf("FLOAT2INT LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name);
}

void gen_int2char(TThreeAC *instruct)
{
    printf("INT2CHAR LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name);
}

void gen_concat(TThreeAC *instruct)
{
    printf("CONCAT LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
}

void gen_setchar(TThreeAC *instruct)
{
    printf("SETCHAR LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
}

void gen_isint(TThreeAC *instruct) //až zjistím jak funguje generátor jmen
{
    char *pom = codegen_temp_id_generator(); //proměnná LF@pom
    printf("DEFVAR LF@%s\n", pom);
    printf("TYPE LF@%s LF@%s\n", pom, instruct->op_1->name);
    printf("EQ LF@%s LF@%s string@int\n",instruct->destination->name , pom);
}

void gen_isfloat(TThreeAC *instruct)
{
    char *pom = codegen_temp_id_generator(); //proměnná LF@pom
    printf("DEFVAR LF@%s\n", pom);
    printf("TYPE LF@%s LF@%s\n", pom, instruct->op_1->name);
    printf("EQ LF@%s LF@%s string@float\n",instruct->destination->name , pom);
}

void gen_isstring(TThreeAC *instruct)
{
    char *pom = codegen_temp_id_generator(); //proměnná LF@pom
    printf("DEFVAR LF@%s\n", pom);
    printf("TYPE LF@%s LF@%s\n", pom, instruct->op_1->name);
    printf("EQ LF@%s LF@%s string@string\n",instruct->destination->name , pom);
}

void gen_isbool(TThreeAC *instruct)
{
    char *pom = codegen_temp_id_generator(); //proměnná LF@pom
    printf("DEFVAR LF@%s\n", pom);
    printf("TYPE LF@%s LF@%s\n", pom, instruct->op_1->name);
    printf("EQ LF@%s LF@%s string@bool\n",instruct->destination->name , pom);
}

void gen_lable(TThreeAC *instruct)
{
    printf("LABEL %s\n",instruct->op_1->name);
}

void gen_deffunc(TThreeAC *instruct)
{
    printf("LABEL %s\n"
           "CREATEFRAME\n"
           "PUSHFRAME\n",instruct->op_1->name);
}

void gen_jump(TThreeAC *instruct)
{
    printf("JUMP %s\n", instruct->op_1->name);
}

void gen_jumpifeq(TThreeAC *instruct)
{
    printf("JUMPIFEQ %s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
}

void gen_jumpifneq(TThreeAC *instruct)
{
    printf("JUMPIFNEQ %s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
}

void gen_jumpifgt(TThreeAC *instruct)
{

}

void gen_jumpiflt(TThreeAC *instruct)
{

}

void gen_dprint(TThreeAC *instruct)
{

}

void gen_eq(TThreeAC *instruct)
{

}

void gen_gt(TThreeAC *instruct)
{

}

void gen_lt(TThreeAC *instruct)
{

}

void gen_gteq(TThreeAC *instruct)
{

}

void gen_lteq(TThreeAC *instruct)
{

}

void gen_neq(TThreeAC *instruct)
{

}

void GEN_start(TTacList *list)
{
    pream();
    TThreeAC *I2 = TAC_remove(list);
    while (I2 != NULL)
    {
        switch (I2->name) {
            case DEFVAR:
                gen_defvar(I2);
                break;
            case MOVE:
                gen_move(I2);
                break;
            case DEFMOVE:
                gen_defmove_const(I2);
                break;
            case CREATEFRAME:
                gen_createframe(I2);
                break;
            case PUSHFRAME:
                gen_pushframe(I2);
                break;
            case POPFRAME:
                gen_popframe(I2);
                break;
            case LOADPARAM:
                gen_loadparam(I2);
                break;
            case PUSH:
                gen_push(I2);
                break;
            case POP:
                gen_pop(I2);
                break;
            case ADD:
                gen_add(I2);
                break;
            case SUB:
                gen_sub(I2);
                break;
            case MUL:
                gen_mul(I2);
                break;
            case DIV:
                gen_div(I2);
                break;
            case CALL:
                gen_call(I2);
                break;
            case RETURN:
                gen_return(I2);
                break;
            case INT2FLOAT:
                gen_int2float(I2);
                break;
            case FLOAT2INT:
                gen_float2int(I2);
                break;
            case INT2CHAR:
                gen_int2char(I2);
                break;
            case CONCAT:
                gen_concat(I2);
                break;
            case SETCHAR:
                gen_setchar(I2);
                break;
            case ISINT:
                gen_isint(I2);
                break;
            case ISFLOAT:
                gen_isfloat(I2);
                break;
            case ISSTRING:
                gen_isstring(I2);
                break;
            case ISBOOL:
                gen_isbool(I2);
                break;
            case LABLE:
                gen_lable(I2);
                break;
            case DEFFUNC:
                gen_deffunc(I2);
                break;
            case JUMP:
                gen_jump(I2);
                break;
            case JUMPIFEQ:
                gen_jumpifeq(I2);
                break;
            case JUMPIFNEQ:
                gen_jumpifneq(I2);
                break;
            case JUMPIFGT:
                gen_jumpifgt(I2);
                break;
            case JUMPIFLT:
                gen_jumpiflt(I2);
                break;
            case DPRINT:
                gen_dprint(I2);
                break;
            case EQ:
                gen_eq(I2);
                break;
            case GT:
                gen_gt(I2);
                break;
            case LT:
                gen_lt(I2);
                break;
            case GTEQ:
                gen_gteq(I2);
                break;
            case LTEQ:
                gen_lteq(I2);
                break;
            case NEQ:
                gen_neq(I2);
                break;
        }
        I2 = TAC_remove(list);
    }
}