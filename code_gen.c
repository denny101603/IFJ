

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
//#include "seman.h"
#include "fsm.h"
//#include <stdio.h>
//#include <stdbool.h>

char *codegen_temp_id_generator()
{
    static unsigned long long cnt = 0;
    char *name = (char *) malloc(sizeof(char)*32);
    if(name == NULL)
        return NULL;
    sprintf(name, "&codegen%llu", cnt++);
    return name;
}

char *whatType(int x)
{
    if(x == 41) //int
    {
        return "int";
    }
    else if(x == 35) //string
    {
        return "string";
    }
    else if(x == 63) //bool
    {
        return "bool";
    }
    else if(x == 44) //float
    {
        return "float";
    }
    else if(x == 54) //nil
    {
        return "nil";
    }

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

    //substr
    printf("LABEL substr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@podretezec\n");
    printf("DEFVAR LF@znak\n");
    printf("DEFVAR LF@s\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@n\n");
    printf("POPS LF@n\n");
    printf("POPS LF@i\n");
    printf("POPS LF@s\n");
    printf("MOVE LF@podretezec LF@znak\n");
    printf("LABEL while2\n");
    printf("JUMPIFEQ while2_end LF@i LF@n\n");
    printf("GETCHAR LF@znak LF@s LF@i\n");
    printf("CONCAT LF@podretezec LF@podretezec LF@znak\n");
    printf("ADD LF@i LF@i int@i\n");
    printf("JUMP while2\n");
    printf("LABEL while2_end\n");
    printf("PUSHS LF@podretezec\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //ord
    printf("LABEL ord\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@hodnota\n");
    printf("DEFVAR LF@s\n");
    printf("DEFVAR LF@i\n");
    printf("POPS LF@i\n");
    printf("POPS LF@s\n");
    printf("STRI2INT LF@hodnota LF@s LF@i\n");
    printf("PUSHS LF@hodnota\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //chr
    printf("LABEL chr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@znak\n");
    printf("DEFVAR LF@s\n");
    printf("POPS LF@i\n");
    printf("INT2CHAR LF@znak LF@i\n");
    printf("PUSHS LF@znak\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    //začátek kódu
    printf("LABEL main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

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
    char *typ = whatType(instruct->op_1->type);
    printf("DEFVAR LF@%s\n", instruct->destination->name);
    printf("MOVE LF@%s %s@%s\n", instruct->destination->name, typ, instruct->op_1->name);
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
    char* a$type = codegen_temp_id_generator();
    char* a$tmp = codegen_temp_id_generator();
    char* b$type = codegen_temp_id_generator();
    char* b$tmp = codegen_temp_id_generator();
    char* a = codegen_temp_id_generator();
    char* b = codegen_temp_id_generator();
    char* mul$a$int = codegen_temp_id_generator();
    char* mul$a$float = codegen_temp_id_generator();
    char* mul$a$int$a$retype = codegen_temp_id_generator();
    char* mul$error$end = codegen_temp_id_generator();
    char* x = codegen_temp_id_generator();
    char* mul$end = codegen_temp_id_generator();
    char* mul$a$float$b$retype = codegen_temp_id_generator();




    printf("DEFVAR LF@%s\n", a$type);
    printf("DEFVAR LF@%s\n", a$tmp);
    printf("DEFVAR LF@%s\n", b$type);
    printf("DEFVAR LF@%s\n", b$tmp);

    printf("TYPE LF@%s LF@%s\n", a$type, a);
    printf("TYPE LF@%s LF@%s\n", b$type, b);

    printf("JUMPIFEQ %s LF@%s string@int\n", mul$a$int, a$type);
    printf("JUMPIFEQ %s LF@%s string@int\n", mul$a$float, a$type);

    printf("LABEL %s\n", mul$a$int);

    printf("JUMPIFEQ %s LF@%s string@float\n", mul$a$int$a$retype, b$type );
    printf("JUMPIFNEQ %s LF@%s string@int\n", mul$error$end,  b$type);

    printf("MUL LF@%s LF@%s LF@%s\n", x, a, b);
    printf("JUMP %s\n",mul$end);

    printf("LABEL %s", mul$a$int$a$retype);
    printf("INT2FLOAT LF@%s LF@%s\n",a$tmp, a);


    printf("MUL LF@%s LF@%s LF@%s", x, a$tmp, b);
    printf("JUMP %s\n", mul$end);


    printf("LABEL %s\n",mul$a$float );
    printf("JUMPIFEQ %s LF@%s string@int\n",mul$a$float$b$retype, b$type );
    printf("JUMPIFNEQ %s LF@%s string@float\n",mul$error$end, b$type );

    printf("MUL LF@%s LF@%s LF@%s\n", x, a, b);

    printf("JUMP %s\n", mul$end);

    printf("LABEL %s\n",mul$a$float$b$retype );
    printf("INT2FLOAT LF@%s LF@%s\n", b$tmp, b);

    printf("MUL LF@%s LF@%s LF@%s\n",x, a, b$tmp );
    printf("JUMP %s\n", mul$end);

    printf("LABEl %s\n", mul$error$end) ;
    printf("EXIT int@4");

    printf("LABEL mul$end");
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
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp
    char *tmp = codegen_temp_id_generator(); //proměnná LF@tmp

    char *eqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *eqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *eqastring = codegen_temp_id_generator(); //label skoku když a je float
    char *eqabool = codegen_temp_id_generator(); //label skoku když a je float
    char *eqanil = codegen_temp_id_generator(); //label skoku když a je float
    char *eqaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqfloatsame = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *eqend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *eqerrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4
    char *eqintsame = codegen_temp_id_generator(); //label skoku

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqafloat, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n", eqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@nil\n", eqanil, atype);
    printf("JUMPIFEQ %s LF@%s string@string\n", eqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n", eqabool, atype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqafloat); //A JE FLOAT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqbretype, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqfloatsame, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqfloatsame); //OBA JSOU FLOAT
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqbretype); //A JE FLOAT B JE INT
    printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, btmp);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaint); //A JE INT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqintsame, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqaretype, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqintsame); //OBA INT
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaretype); //PŘETYPOVAT A
    printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, atmp, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqanil); //POROVNÁNÍ NILŮ
    printf("JUMPIFNEQ %s LF@%s string@nil\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqabool); //POROVNÁNÍ BOOLŮ
    printf("JUMPIFNEQ %s LF@%s string@bool\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqastring); //POROVNÁNÍ STRINGŮ
    printf("JUMPIFNEQ %s LF@%s string@string\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqerrorend);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", eqend);
    printf("JUMPIFEQ %s LF@%s bool@true", instruct->destination->name, tmp);
}

void gen_jumpifneq(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp
    char *tmp = codegen_temp_id_generator(); //proměnná LF@tmp

    char *eqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *eqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *eqastring = codegen_temp_id_generator(); //label skoku když a je float
    char *eqabool = codegen_temp_id_generator(); //label skoku když a je float
    char *eqanil = codegen_temp_id_generator(); //label skoku když a je float
    char *eqaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqfloatsame = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *eqend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *eqerrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4
    char *eqintsame = codegen_temp_id_generator(); //label skoku

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqafloat, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n", eqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@nil\n", eqanil, atype);
    printf("JUMPIFEQ %s LF@%s string@string\n", eqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n", eqabool, atype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqafloat); //A JE FLOAT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqbretype, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqfloatsame, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqfloatsame); //OBA JSOU FLOAT
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqbretype); //A JE FLOAT B JE INT
    printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, btmp);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaint); //A JE INT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqintsame, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqaretype, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqintsame); //OBA INT
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaretype); //PŘETYPOVAT A
    printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, atmp, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqanil); //POROVNÁNÍ NILŮ
    printf("JUMPIFNEQ %s LF@%s string@nil\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqabool); //POROVNÁNÍ BOOLŮ
    printf("JUMPIFNEQ %s LF@%s string@bool\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqastring); //POROVNÁNÍ STRINGŮ
    printf("JUMPIFNEQ %s LF@%s string@string\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqerrorend);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", eqend);
    printf("JUMPIFNEQ %s LF@%s bool@true", instruct->destination->name, tmp);
}

void gen_jumpifgt(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp
    char *tmp = codegen_temp_id_generator(); //proměnná LF@tmp

    char *gtastring = codegen_temp_id_generator(); //label skoku když a je string
    char *gtabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *gtaint = codegen_temp_id_generator(); //label skoku když a je int
    char *gtaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *gtaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *gtafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *gtafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *gtafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *gterror = codegen_temp_id_generator(); //label skoku když error
    char *gtend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",gtastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n",gtabool , atype);
    printf("JUMP %s\n",gterror);


    printf("LABEL %s\n", gtafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtafloatsame , btype);
    printf("JUMP %s\n",gterror);

    printf("LABEL %s\n", gtafloatsame);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);

    printf("LABEL %s\n", gtafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, btmp);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtaintretype , btype);
    printf("JUMP %s\n",gterror);

    printf("LABEL %s\n", gtaintsame);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);

    printf("LABEL %s\n", gtaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("GT LF%s LF%s LF%s\n", tmp, atmp, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",gterror , btype);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",gterror , btype);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gterror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", gtend);
    printf("JUMPIFEQ %s LF@%s bool@true\n", instruct->destination->name, tmp);}

void gen_jumpiflt(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp
    char *tmp = codegen_temp_id_generator(); //proměnná LF@tmp

    char *ltastring = codegen_temp_id_generator(); //label skoku když a je string
    char *ltabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *ltaint = codegen_temp_id_generator(); //label skoku když a je int
    char *ltaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *ltaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *ltafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *ltafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *ltafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *lterror = codegen_temp_id_generator(); //label skoku když error
    char *ltend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",ltastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",ltaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",ltafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n",ltabool , atype);
    printf("JUMP %s\n",lterror);


    printf("LABEL %s\n", ltafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",ltafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",ltafloatsame , btype);
    printf("JUMP %s\n",lterror);

    printf("LABEL %s\n", ltafloatsame);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",ltend);

    printf("LABEL %s\n", ltafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, btmp);
    printf("JUMP %s\n",ltend);


    printf("LABEL %s\n", ltaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",ltaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",ltaintretype , btype);
    printf("JUMP %s\n",lterror);

    printf("LABEL %s\n", ltaintsame);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",ltend);

    printf("LABEL %s\n", ltaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("GT LF%s LF%s LF%s\n", tmp, atmp, instruct->op_2->name);
    printf("JUMP %s\n",ltend);


    printf("LABEL %s\n", ltastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",lterror , btype);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",ltend);


    printf("LABEL %s\n", ltabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",lterror , btype);
    printf("GT LF%s LF%s LF%s\n", tmp, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",ltend);


    printf("LABEL %s\n", lterror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", ltend);
    printf("JUMPIFEQ %s LF@%s bool@true\n", instruct->destination->name, tmp);
}

void gen_dprint(TThreeAC *instruct)
{
    char *typ = whatType(instruct->op_1->type);
    printf("DPRINT %s@%s\n", typ, instruct->op_1->name);
}

void gen_eq(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //proměnná LF@atype
    char *atmp = codegen_temp_id_generator(); //proměnná LF@atmp

    char *btype = codegen_temp_id_generator(); //proměnná LF@btype
    char *btmp = codegen_temp_id_generator(); //proměnná LF@btmp

    char *eqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *eqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *eqastring = codegen_temp_id_generator(); //label skoku když a je float
    char *eqabool = codegen_temp_id_generator(); //label skoku když a je float
    char *eqanil = codegen_temp_id_generator(); //label skoku když a je float
    char *eqaretype = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqfloatsame = codegen_temp_id_generator(); //label skoku když a je int "b" float -> nutno přetypovat a na float
    char *eqbretype = codegen_temp_id_generator(); //label skoku když a je float a b int -> nutno přetypovat b na float
    char *eqend = codegen_temp_id_generator(); //label skoku na konec instrukce add (po provedení add)
    char *eqerrorend = codegen_temp_id_generator(); //label skoku když dojde k typové chybě pro ukončení s návratovým kódem 4
    char *eqintsame = codegen_temp_id_generator();

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqafloat, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n", eqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@nil\n", eqanil, atype);
    printf("JUMPIFEQ %s LF@%s string@string\n", eqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n", eqabool, atype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqafloat); //A JE FLOAT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqbretype, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqfloatsame, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqfloatsame); //OBA JSOU FLOAT
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqbretype); //A JE FLOAT B JE INT
    printf("INT2FLOAT LF@%s LF@%s\n", btmp, instruct->op_2->name);
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, btmp);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaint); //A JE INT
    printf("JUMPIFEQ %s LF@%s string@int\n", eqintsame, btype);
    printf("JUMPIFEQ %s LF@%s string@float\n", eqaretype, btype);
    printf("JUMP %s \n", eqerrorend);

    printf("LABEL %s\n", eqintsame); //OBA INT
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqaretype); //PŘETYPOVAT A
    printf("INT2FLOAT LF@%s LF@%s\n", atmp, instruct->op_1->name);
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, atmp, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqanil); //POROVNÁNÍ NILŮ
    printf("JUMPIFNEQ %s LF@%s string@nil\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqabool); //POROVNÁNÍ BOOLŮ
    printf("JUMPIFNEQ %s LF@%s string@bool\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);

    printf("LABEL %s\n", eqastring); //POROVNÁNÍ STRINGŮ
    printf("JUMPIFNEQ %s LF@%s string@string\n", eqerrorend, btype);
    printf("EQ LF@%s LF@%s LF@%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s \n", eqend);
}

void gen_gt(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp

    char *gtastring = codegen_temp_id_generator(); //label skoku když a je string
    char *gtabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *gtaint = codegen_temp_id_generator(); //label skoku když a je int
    char *gtaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *gtaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *gtafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *gtafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *gtafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *gterror = codegen_temp_id_generator(); //label skoku když error
    char *gtend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",gtastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n",gtabool , atype);
    printf("JUMP %s\n",gterror);


    printf("LABEL %s\n", gtafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtafloatsame , btype);
    printf("JUMP %s\n",gterror);

    printf("LABEL %s\n", gtafloatsame);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);

    printf("LABEL %s\n", gtafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, btmp);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",gtaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gtaintretype , btype);
    printf("JUMP %s\n",gterror);

    printf("LABEL %s\n", gtaintsame);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);

    printf("LABEL %s\n", gtaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, atmp, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",gterror , btype);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gtabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",gterror , btype);
    printf("GT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("JUMP %s\n",gtend);


    printf("LABEL %s\n", gterror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", gtend);
}

void gen_lt(TThreeAC *instruct)
{
        char *atype = codegen_temp_id_generator(); //promìnná LF@atype
        char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
        char *btype = codegen_temp_id_generator(); //promìnná LF@btype
        char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp

        char *ltastring = codegen_temp_id_generator(); //label skoku když a je string
        char *ltabool = codegen_temp_id_generator(); //label skoku když a je bool
        char *ltaint = codegen_temp_id_generator(); //label skoku když a je int
        char *ltaintretype = codegen_temp_id_generator(); //label skoku když intretype
        char *ltaintsame = codegen_temp_id_generator(); //label skoku když intsame
        char *ltafloat = codegen_temp_id_generator(); //label skoku když a je float
        char *ltafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
        char *ltafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
        char *lterror = codegen_temp_id_generator(); //label skoku když error
        char *ltend = codegen_temp_id_generator(); //label skoku když end

        printf("DEFVAR LF@%s\n", atype);
        printf("DEFVAR LF@%s\n", atmp);
        printf("DEFVAR LF@%s\n", btype);
        printf("DEFVAR LF@%s\n", btmp);

        printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
        printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

        printf("JUMPIFEQ %s LF@%s string@string\n",ltastring, atype);
        printf("JUMPIFEQ %s LF@%s string@int\n",ltaint, atype);
        printf("JUMPIFEQ %s LF@%s string@float\n",ltafloat , atype);
        printf("JUMPIFEQ %s LF@%s string@bool\n",ltabool , atype);
        printf("JUMP %s\n",lterror);


        printf("LABEL %s\n", ltafloat);
        printf("JUMPIFEQ %s LF@%s string@int\n",ltafloatretype , btype);
        printf("JUMPIFEQ %s LF@%s string@float\n",ltafloatsame , btype);
        printf("JUMP %s\n",lterror);

        printf("LABEL %s\n", ltafloatsame);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n",ltend);

        printf("LABEL %s\n", ltafloatretype);
        printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, btmp);
        printf("JUMP %s\n",ltend);


        printf("LABEL %s\n", ltaint);
        printf("JUMPIFEQ %s LF@%s string@int\n",ltaintsame , btype);
        printf("JUMPIFEQ %s LF@%s string@float\n",ltaintretype , btype);
        printf("JUMP %s\n",lterror);

        printf("LABEL %s\n", ltaintsame);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n",ltend);

        printf("LABEL %s\n", ltaintretype);
        printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, atmp, instruct->op_2->name);
        printf("JUMP %s\n",ltend);


        printf("LABEL %s\n", ltastring);
        printf("JUMPIFNEQ %s LF@%s string@string\n",lterror , btype);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n",ltend);


        printf("LABEL %s\n", ltabool);
        printf("JUMPIFNEQ %s LF@%s string@bool\n",lterror , btype);
        printf("LT LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
        printf("JUMP %s\n",ltend);


        printf("LABEL %s\n", lterror);
        printf("EXIT int@4\n");

        printf("LABEL %s\n", ltend);
}

void gen_gteq(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *xgt = codegen_temp_id_generator(); //promìnná LF@agt
    char *xeq = codegen_temp_id_generator(); //promìnná LF@aeq
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp

    char *gteqastring = codegen_temp_id_generator(); //label skoku když a je string
    char *gteqabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *gteqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *gteqaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *gteqaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *gteqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *gteqafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *gteqafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *gteqerror = codegen_temp_id_generator(); //label skoku když error
    char *gteqend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", xgt);
    printf("DEFVAR LF@%s\n", xeq);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",gteqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",gteqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gteqafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gteqabool , atype);
    printf("JUMP %s\n",gteqerror);


    printf("LABEL %s\n", gteqafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",gteqafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gteqafloatsame , btype);
    printf("JUMP %s\n",gteqerror);

    printf("LABEL %s\n", gteqafloatsame);
    printf("GT LF%s LF%s LF%s\n", xgt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);

    printf("LABEL %s\n", gteqafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("GT LF%s LF%s LF%s\n", xgt, instruct->op_1->name, btmp);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, btmp);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);


    printf("LABEL %s\n", gteqaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",gteqaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",gteqaintretype , btype);
    printf("JUMP %s\n",gteqerror);

    printf("LABEL %s\n", gteqaintsame);
    printf("GT LF%s LF%s LF%s\n", xgt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);

    printf("LABEL %s\n", gteqaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("GT LF%s LF%s LF%s\n", xgt, atmp, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, atmp, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);


    printf("LABEL %s\n", gteqastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",gteqerror , btype);
    printf("GT LF%s LF%s LF%s\n", xgt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);


    printf("LABEL %s\n", gteqabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",gteqerror , btype);
    printf("GT LF%s LF%s LF%s\n", xgt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xgt, xeq);
    printf("JUMP %s\n",gteqend);


    printf("LABEL %s\n", gteqerror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", gteqend);

}

void gen_lteq(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *xlt = codegen_temp_id_generator(); //promìnná LF@alt
    char *xeq = codegen_temp_id_generator(); //promìnná LF@aeq
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp

    char *lteqastring = codegen_temp_id_generator(); //label skoku když a je string
    char *lteqabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *lteqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *lteqaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *lteqaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *lteqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *lteqafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *lteqafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *lteqerror = codegen_temp_id_generator(); //label skoku když error
    char *lteqend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", xlt);
    printf("DEFVAR LF@%s\n", xeq);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",lteqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",lteqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",lteqafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@bool\n",lteqabool , atype);
    printf("JUMP %s\n",lteqerror);


    printf("LABEL %s\n", lteqafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",lteqafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",lteqafloatsame , btype);
    printf("JUMP %s\n",lteqerror);

    printf("LABEL %s\n", lteqafloatsame);
    printf("LT LF%s LF%s LF%s\n", xlt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);

    printf("LABEL %s\n", lteqafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("LT LF%s LF%s LF%s\n", xlt, instruct->op_1->name, btmp);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, btmp);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);


    printf("LABEL %s\n", lteqaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",lteqaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",lteqaintretype , btype);
    printf("JUMP %s\n",lteqerror);

    printf("LABEL %s\n", lteqaintsame);
    printf("LT LF%s LF%s LF%s\n", xlt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);

    printf("LABEL %s\n", lteqaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("LT LF%s LF%s LF%s\n", xlt, atmp, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, atmp, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);


    printf("LABEL %s\n", lteqastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",lteqerror , btype);
    printf("LT LF%s LF%s LF%s\n", xlt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);


    printf("LABEL %s\n", lteqabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",lteqerror , btype);
    printf("LT LF%s LF%s LF%s\n", xlt, instruct->op_1->name, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", xeq, instruct->op_1->name, instruct->op_2->name);
    printf("OR LF%s LF%s LF%s\n", instruct->destination->name, xlt, xeq);
    printf("JUMP %s\n",lteqend);


    printf("LABEL %s\n", lteqerror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", lteqend);
}

void gen_neq(TThreeAC *instruct)
{
    char *atype = codegen_temp_id_generator(); //promìnná LF@atype
    char *atmp = codegen_temp_id_generator(); //promìnná LF@atmp
    char *btype = codegen_temp_id_generator(); //promìnná LF@btype
    char *btmp = codegen_temp_id_generator(); //promìnná LF@btmp

    char *neqastring = codegen_temp_id_generator(); //label skoku když a je string
    char *neqabool = codegen_temp_id_generator(); //label skoku když a je bool
    char *neqanil = codegen_temp_id_generator(); //label skoku když a je nil
    char *neqaint = codegen_temp_id_generator(); //label skoku když a je int
    char *neqaintretype = codegen_temp_id_generator(); //label skoku když intretype
    char *neqaintsame = codegen_temp_id_generator(); //label skoku když intsame
    char *neqafloat = codegen_temp_id_generator(); //label skoku když a je float
    char *neqafloatretype = codegen_temp_id_generator(); //label skoku když floatretype
    char *neqafloatsame = codegen_temp_id_generator(); //label skoku když floatsame
    char *neqerror = codegen_temp_id_generator(); //label skoku když error
    char *neqend = codegen_temp_id_generator(); //label skoku když end

    printf("DEFVAR LF@%s\n", atype);
    printf("DEFVAR LF@%s\n", atmp);
    printf("DEFVAR LF@%s\n", btype);
    printf("DEFVAR LF@%s\n", btmp);

    printf("TYPE LF@%s LF@%s\n", atype, instruct->op_1->name);
    printf("TYPE LF@%s LF@%s\n", btype, instruct->op_2->name);

    printf("JUMPIFEQ %s LF@%s string@string\n",neqastring, atype);
    printf("JUMPIFEQ %s LF@%s string@int\n",neqaint, atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",neqafloat , atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",neqabool , atype);
    printf("JUMPIFEQ %s LF@%s string@float\n",neqanil , atype);
    printf("JUMP %s\n",neqerror);


    printf("LABEL %s\n", neqafloat);
    printf("JUMPIFEQ %s LF@%s string@int\n",neqafloatretype , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",neqafloatsame , btype);
    printf("JUMP %s\n",neqerror);

    printf("LABEL %s\n", neqafloatsame);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);

    printf("LABEL %s\n", neqafloatretype);
    printf("INT2FLOAT LF%s LF%s\n", btmp, instruct->op_2->name);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, btmp);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);


    printf("LABEL %s\n", neqaint);
    printf("JUMPIFEQ %s LF@%s string@int\n",neqaintsame , btype);
    printf("JUMPIFEQ %s LF@%s string@float\n",neqaintretype , btype);
    printf("JUMP %s\n",neqerror);

    printf("LABEL %s\n", neqaintsame);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);

    printf("LABEL %s\n", neqaintretype);
    printf("INT2FLOAT LF%s LF%s\n", atmp, instruct->op_1->name);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, atmp, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);


    printf("LABEL %s\n", neqastring);
    printf("JUMPIFNEQ %s LF@%s string@string\n",neqerror , btype);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);


    printf("LABEL %s\n", neqabool);
    printf("JUMPIFNEQ %s LF@%s string@bool\n",neqerror , btype);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);


    printf("LABEL %s\n", neqanil);
    printf("JUMPIFNEQ %s LF@%s string@nil\n",neqerror , btype);
    printf("EQ LF%s LF%s LF%s\n", instruct->destination->name, instruct->op_1->name, instruct->op_2->name);
    printf("NOT LF%s LF%s\n",instruct->destination->name, instruct->destination->name);
    printf("JUMP %s\n",neqend);


    printf("LABEL %s\n", neqerror);
    printf("EXIT int@4\n");

    printf("LABEL %s\n", neqend);
}

void GEN_start(TTacList *list)
{
    TThreeAC *I2 = list->first;
    TThreeAC *first_while;
    int while_count = 0;

    //generace .IFJcode18 a podobné píčoviny
    pream();

    while(I2 != NULL)
    {
        if(I2->name == WHILE)
        {
            if(while_count == 0)
                first_while = I2;
            while_count++;
        }
        if(I2->name == ENDWHILE)
        {
            if(while_count != 0)
                while_count--;
        }
        if((I2->name == DEFVAR && while_count != 0) || (I2->name == DEFMOVE && while_count != 0))
        {
            I2 = TAC_remove_this(list, I2);
            TAC_insert_post(list, first_while, I2);
        }
        I2 = I2->next;
    }

    I2 = TAC_remove(list);
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
        TThreeAC_delete(I2);
        I2 = TAC_remove(list);
    }
}