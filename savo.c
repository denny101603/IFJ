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
*	@file savo.c
*	@author Jan Beran
*	@brief Soubor se syntaktickym analyzatorem vyrazu
 *	v1.7: Savo funkcni v 99 % pripadu. V nalsedujicim commitu bude refaktorovano.
*/
#include "savo.h"
#include "symtable.h"


#define NUM_OF_RULES 22
#define RULE_LENGTH 3

/**
 * @brief Tabulka pravidel pouzivana funkci get_action.
 * @author Matej Jelinek, Jan Beran
 * @note V2.0 vetsina pravidel odstranena, mozna to prestane fungoovat :)
 */
int rules[NUM_OF_RULES][RULE_LENGTH] = { //E->
        {0,0,INTEGER}, //0
        {0,0,EXPRESSION}, //1
        {0,0,FLOAT_2}, //2
        {0,0,STRING_1}, //3
        {0, 0,ID_2},//4
        {0,0,KEY_NIL},//5
        {LEFT_BRACKET, INTEGER, RIGHT_BRACKET},//6
        {LEFT_BRACKET, EXPRESSION, RIGHT_BRACKET},//7
        {LEFT_BRACKET, FLOAT_2, RIGHT_BRACKET},//8
        {LEFT_BRACKET, STRING_1, RIGHT_BRACKET},//9
        {LEFT_BRACKET, ID_2, RIGHT_BRACKET},//10
        {EXPRESSION, OP_PLUS, EXPRESSION},//11
        {EXPRESSION, OP_MINUS, EXPRESSION},//12
        {EXPRESSION, OP_MULT, EXPRESSION},//13
        {EXPRESSION, OP_DIV, EXPRESSION}, //14
        {EXPRESSION, OP_MORE_1, EXPRESSION}, //15
        {EXPRESSION, OP_LESS_1, EXPRESSION},//16
        {EXPRESSION, OP_EQAL_1, EXPRESSION},//17
        {EXPRESSION, OP_LESS_EQUAL, EXPRESSION},//18
        {EXPRESSION, OP_MORE_EQUAL, EXPRESSION},//19
        {EXPRESSION, OP_NOT_EQ_1, EXPRESSION}, //20
        {LEFT_BRACKET, KEY_NIL, RIGHT_BRACKET}//21
};

/**
 * @brief Precedencni tbaulka pro syntaktickou analyzu vyrazu.
 * @author Jan Carba, Jan Beran
 */
char prec_table[16][16] = {              //in
        /*           + 	  -   *	  /	  (	  )	 id	 con nil  <   >  <=	  >=  !=  ==   $ */
        /* + */    {'>','>','<','<','<','>','<','<','?','>','>','>','>','>','>','>'},
        /* - */    {'>','>','<','<','<','>','<','<','?','>','>','>','>','>','>','>'},
        /* * */    {'>','>','>','>','<','>','<','<','?','>','>','>','>','>','>','>'},
        /* / */    {'>','>','>','>','<','>','<','<','?','>','>','>','>','>','>','>'},
        /* ( */    {'<','<','<','<','<','=','<','<','<','<','<','<','<','<','<','?'},
        /* ) */    {'>','>','>','>','?','>','?','?','?','>','>','>','>','>','>','>'},
        /* id */   {'>','>','>','>','?','>','?','?','?','>','>','>','>','>','>','>'},
/*stack  cons */   {'>','>','>','>','?','>','?','?','?','>','>','>','>','>','>','>'},
        /*nil*/    {'?','?','?','?','?','>','?','?','?','?','?','?','?','>','>','>'},
        /* < */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','?','>'},
        /* > */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','?','>'},
        /* <= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','?','>'},
        /* >= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','?','>'},
        /* != */   {'<','<','<','<','<','>','<','<','<','?','?','?','?','?','?','>'},
        /* == */   {'<','<','<','<','<','>','<','<','<','?','?','?','?','?','?','>'},
        /* $ */    {'<','<','<','<','<','?','<','<','<','<','<','<','<','<','<','?'}
};
//TODO ALL Zkontrolovat to po Janu Carbovi :). By Berry: pokud bude savo naoko fungovat, ale jinak, mrknout se sem.


char get_action(Ttoken *input_token, Ttoken *stack_token)
{
    int stack_terminal = -1;
    int input_terminal = -1;
    /* + 	  -   *	  /	  (	  )	 id	 con  <   >  <=	  >=  !=  ==   $ */
    switch(input_token->type)
    {
        case OP_PLUS:
            input_terminal = 0;
            break;
        case OP_MINUS:
            input_terminal = 1;
            break;
        case OP_MULT:
            input_terminal = 2;
            break;
        case OP_DIV:
            input_terminal = 3;
            break;
        case LEFT_BRACKET:
            input_terminal = 4;
            break;
        case RIGHT_BRACKET:
            input_terminal = 5;
            break;
        case ID_2:
            input_terminal = 6;
            break;
        case INTEGER:
        case STRING_1:
        case FLOAT_2:
            input_terminal = 7;
            break;
        case KEY_NIL:
            input_terminal = 8;
                break;
        case OP_LESS_1:
            input_terminal = 9;
            break;
        case OP_MORE_1:
            input_terminal = 10;
            break;
        case OP_LESS_EQUAL:
            input_terminal = 11;
            break;
        case OP_MORE_EQUAL:
            input_terminal = 12;
            break;
        case OP_NOT_EQ_1:
            input_terminal = 13;
            break;
        case OP_EQAL_1:
            input_terminal = 14;
            break;
        case EOF_STATE: //upravovano
        case EOL_1:
        case OP_COMMA:
        case KEY_THEN:
        case KEY_DO:
            input_terminal = 15;
            break;
        default:
            input_terminal = -1;
            break;
    }
    switch (stack_token->type)
    {
        case OP_PLUS:
            stack_terminal = 0;
            break;
        case OP_MINUS:
            stack_terminal = 1;
            break;
        case OP_MULT:
            stack_terminal = 2;
            break;
        case OP_DIV:
            stack_terminal = 3;
            break;
        case LEFT_BRACKET:
            stack_terminal = 4;
            break;
        case RIGHT_BRACKET:
            stack_terminal = 5;
            break;
        case ID_2:
            stack_terminal = 6;
            break;
        case INTEGER:
        case STRING_1:
        case FLOAT_2:
            stack_terminal = 7;
            break;
        case KEY_NIL:
            stack_terminal = 8;
            break;
        case OP_LESS_1:
            stack_terminal = 9;
            break;
        case OP_MORE_1:
            stack_terminal = 10;
            break;
        case OP_LESS_EQUAL:
            stack_terminal = 11;
            break;
        case OP_MORE_EQUAL:
            stack_terminal = 12;
            break;
        case OP_NOT_EQ_1:
            stack_terminal = 13;
            break;
        case OP_EQAL_1:
            stack_terminal = 14;
            break;
        case BOTTOM_TOKEN:
            stack_terminal = 15;
            break;
        default:
            stack_terminal = -1;
            break;
    }
    if (input_terminal == -1 || stack_terminal == -1)
        return '?'; //neni zaznam v tabulce
    return prec_table[stack_terminal][input_terminal];
}

bool is_terminus(Ttoken *token)
{
    if(token->type == EOF_STATE ||
       token->type == EOL_1 ||
       token->type == OP_COMMA ||
       token->type == KEY_THEN ||
       token->type == KEY_DO ||
       token->type == RIGHT_BRACKET
       )
        return true;
    return false;
}

TStack *stack_init()
{
    TStack *stack = (TStack *) malloc(sizeof(TStack));
    if(stack == NULL)
        return NULL;
    //fprintf(stderr,"init token");
    TStackElem *top = (TStackElem *) malloc(sizeof(TStackElem));
    if(top == NULL)
    {
        return NULL;
    }
    Ttoken *first = (Ttoken *)malloc(sizeof(Ttoken));
    if(first == NULL)
    {
        return NULL;
    }
    first->type = BOTTOM_TOKEN;
    first->attribute = NULL;
    top->data = first;
    top->next = NULL;
    top->prev = NULL;
    stack->top = top;
    //fprintf(stderr,"token je: %d \n", stack->top->data->type);
    return stack;
}

bool push(TStack *stack, TStackElem *stack_elem, Ttoken *input_token, Toperand *op)
{
    TStackElem *inputed = (TStackElem *) malloc(sizeof(TStackElem));
    if(inputed == NULL)
    {
        return false;
    }

    inputed->data = input_token;
    inputed->next = stack_elem->next;
    inputed->prev = stack_elem;
    inputed->operand = op;

    if(stack->top == stack_elem)
        stack->top = inputed;

    if(stack_elem->next != NULL)
        stack_elem->next->prev = inputed;
    stack_elem->next = inputed;

   // stack->top = inputed;
    //printf("test: %d %d\n",stack->top->data->type, token->type);
    return true;
}

Ttoken *pop(TStack *stack)
{
    if (stack == NULL)
        return NULL;
    TStackElem *temp = stack->top;
    stack->top = stack->top->prev;
    stack->top->next = NULL;
    Ttoken *output = temp->data;
    free(temp);
    return output;
}

Ttoken *pop_extended(TStack *stack, Toperand *op)
{
    if (stack == NULL)
        return NULL;
    TStackElem *temp = stack->top;
    stack->top = stack->top->prev;
    stack->top->next = NULL;
    Ttoken *output = temp->data;

    if(temp->operand != NULL)
        op = temp->operand;

    free(temp);
    return output;
}

bool copy_buffer(TBuffer *src, TBuffer *dst)
{
    if(src == NULL)
        return true;
    Ttoken *tempsrc = buffer_popTop(src);
    while(src->top != NULL) //updated via poptop
    {
        buffer_push_bottom(dst, tempsrc);
        tempsrc = buffer_popTop(src);
    }
    buffer_push_bottom(dst, tempsrc);
    return true;
}



TStackElem *get_first_terminal(TStack *stack)
{
    if (stack == NULL)
        return NULL;
    //fprintf(stderr, "jsme ve fci get first terminal.\n");
    TStackElem *temp = stack->top;
    while (temp->data->type == EXPRESSION || temp->data->type == ACTION_MENSITKO)
    {
        temp = temp->prev;
    }
    //fprintf(stderr, "koncim get_first_terminal, vystup je: %d\n",temp->data->type);
    return temp;
}

Ttoken *get_token_from_elem(TStackElem *elem)
{
    //fprintf(stderr, "*********************** %d\n", elem->data->type);
    return elem->data;
}

void delete_stack(TStack *stack)
{
    TStackElem *temp = NULL;// = stack->top;
    while (stack->top != NULL)
    {
        temp = stack->top;
        stack->top = stack->top->prev;
        if(temp != NULL) free(temp);
    }
}
char *savo_name_generator()
{
    static unsigned long long cnt = 0;
    char *name = (char *) malloc(sizeof(char)*32);
    if(name == NULL)
        return NULL;
    sprintf(name, "&savo%llu", cnt++);
    return name;
}

Ttoken *action_push(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    push(stack, stack->top,input_token, NULL);

    Ttoken *ret = get_next_token(sa_vars);
    if(ret->type == FLOAT_2) // cislo
    {
        if(strtof(ret->attribute, NULL) < 0) //todo Denny by berry. Co mam vracet za err pri spatnem cisle?
        {
            action_err(stack, sa_vars, ERR_SEM_MISC, internal_buffer);
            return  NULL;
        }
    }
    if(ret->type == INTEGER && strtol(ret->attribute, NULL, 10)<0)
    {
        action_err(stack, sa_vars, ERR_SEM_MISC, internal_buffer);
        return NULL;
    }
    if(ret->type == LEX_ERROR)
    {
        action_err(stack, sa_vars, LEX_ERROR, internal_buffer);
        return  NULL;
    }
    buffer_push_top(internal_buffer, ret);
    //pokud jsem dostal nil jako dalsi token v poradi A pred nim neni zavorka NEBO je nil na topu (byl dostan) A soucasny znak neni ukoncovaci
    /*if ((ret->type == KEY_NIL && stack->top->data->type != LEFT_BRACKET)|| (stack->top->data->type == KEY_NIL && !is_terminus(ret)))
    {
        action_err(stack, sa_vars, ERR_SEM_TYPE, internal_buffer);
        return NULL;
    }*/
    //fprintf(stderr,"\n\n*******INPUT TOKEN JE: %d\n\n", ret->type);
    return ret;
}

//action change = akce <
Ttoken *action_change(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    Ttoken *mensitko = malloc(sizeof(Ttoken));
    if(mensitko == NULL)
        return NULL;
    mensitko->type = ACTION_MENSITKO;

    push(stack,get_first_terminal(stack), mensitko, NULL); //pushneme mensitko za prvni terminal
    push(stack, stack->top, input_token, NULL); //push input tokenu na top

    //nacteme dalsi token a okamzite ukladame do interniho bufferu
    Ttoken *ret = get_next_token(sa_vars);
    if(ret->type == FLOAT_2) // cislo
    {
        if(strtof(ret->attribute, NULL) < 0) //todo Denny by berry. Co mam vracet za err pri spatnem cisle?
        {
            action_err(stack, sa_vars, ERR_SEM_MISC, internal_buffer);
            return  NULL;
        }
    }
    if(ret->type == INTEGER && strtol(ret->attribute, NULL, 10)<0)
    {
        action_err(stack, sa_vars, ERR_SEM_MISC, internal_buffer);
        return NULL;
    }
    if(ret->type == LEX_ERROR)
    {
        action_err(stack, sa_vars, LEX_ERROR, internal_buffer);
        return  NULL;
    }
    buffer_push_top(internal_buffer, ret);
    //pokud jsem dostal nil jako dalsi token v poradi A pred nim neni zavorka NEBO je nil na topu (byl dostan) A soucasny znak neni ukoncovaci
    /*if ((ret->type == KEY_NIL && stack->top->data->type != LEFT_BRACKET)|| (stack->top->data->type == KEY_NIL && !is_terminus(ret)))
    {
        action_err(stack, sa_vars, ERR_SEM_TYPE, internal_buffer);
        return NULL;
    }*/
    return ret;
}

bool action_reduce(TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    int rule = find_rule(stack);

    if(rule != -1)
        return execute_rule(rule, stack, sa_vars, internal_buffer);
    else
        return false;
}

int action_err(TStack *stack, TSynCommon *sa_vars, int error, TBuffer *internal_buffer)
{
    fprintf(stderr, "savo err %d\n", error);
    //ERR_SYN je osetrovan pouhym vracenim false:
    // sax obcas dava savu i nerelevantni vstup s cilem zjistit, jestli je to vyraz. Toto nema zpusobit pad.
    if (error != ERR_SYN)
        sa_vars->err_code = error;

    if (stack != NULL)
    {
        delete_stack(stack);
        free(stack);
    }

    copy_buffer(internal_buffer, sa_vars->buffer); //presunuti interniho bufferu do spolecneho se sax
    delete_buffer(internal_buffer);
    free(internal_buffer);

    switch (error) // err_sem_param nenastane
    {
        case ERR_LEX:
            fprintf(stderr, MESSAGE_LEX);
            break;
        case ERR_SYN:
            fprintf(stderr, MESSAGE_SYN);
            break;
        case ERR_SEM_DEF:
            fprintf(stderr, MESSAGE_SEM_DEF);
            break;
        case ERR_SEM_TYPE:
            fprintf(stderr, MESSAGE_SEM_TYPE);
            break;
        case ERR_SEM_MISC:
            fprintf(stderr, MESSAGE_SEM_MISC);
            break;
        case ERR_ZERO_DIVISION:
            fprintf(stderr, MESSAGE_ZERO_DIVISION);
            break;
        case ERR_INTERNAL:
            fprintf(stderr, MESSAGE_INTERNAL_ERROR);
            break;
        default:
            break;
    }

    return error;
}

int find_rule(TStack *stack)
{
    int rule[3] = {0,0,0};
    int ret = -1;

    TStackElem *temp = stack->top;
    //dokud neprojdu vrchni tri tokeny nebo nenarazim na ACTION_MENSITKO
    for(int index = 2; index > -1 && temp->data->type != ACTION_MENSITKO; index-- )
    {
        rule[index] = temp->data->type;
        if(temp->prev != NULL)
            temp = temp->prev;
    }
        for( int i = 0; i < NUM_OF_RULES; i++)
        {
            for(int j = 0; j < RULE_LENGTH; j++)
            {
                if (rule[j] != rules[i][j]) //pravidlo se nerovna
                {
                    /*mozna delete
                    rule[0] = 0;
                    rule[1] = 0;
                    rule[2] = 0; //vynulovani pole pravidel
                     */
                    break;
                }
                else if (rule[j] == rules[i][j] && j == RULE_LENGTH-1) // cele pravidlo se rovna
                    ret = i;
            }
        }
    return ret;

}

bool is_pseudotoken(Ttoken *token)
{
    bool ret = false;
    if((token->type == EXPRESSION) ||
        (token->type == BOTTOM_TOKEN) ||
        (token->type == ACTION_MENSITKO) ||
        (token->type == ACTION_VETSITKO))
        ret = true;
    return ret;
}
bool execute_rule(int rule, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    //pole tokenu pro semanticke akce. Poradi tokenu je takove, ze na [0] je op1, na [2] operator a na [3] op2
    Ttoken *rule_tokens[RULE_LENGTH] = {NULL, NULL, NULL}; //RULE_LENGHT == 3
   //pole operandu korespondujici s polem tokenu
    Toperand *operands[RULE_LENGTH] = {NULL, NULL, NULL};

    //operandy pro semanticke akce
    Toperand *operand1 = malloc(sizeof(Toperand));
    Toperand *operand2 = malloc(sizeof(Toperand));
    Toperand *operand3 = malloc(sizeof(Toperand));
    Toperand *dest = malloc(sizeof(Toperand));
    Tsymbol_table_item *item = NULL;

    //syntakticka predkontrola, protoze syntakticka analyza je destruktivni
    for (int i = 0; i <RULE_LENGTH; i++) //RULE_LENGHT == 3
    {
        if(rules[rule][i] == 0)
            continue;
        else
        {
            switch(i)
            {
                case 0:
                    operand3 = stack->top->operand;
                    rule_tokens[RULE_LENGTH-1-i] = pop(stack);
                    break;
                case 1:
                    operand2 = stack->top->operand;
                    rule_tokens[RULE_LENGTH-1-i] = pop(stack);
                    break;
                case 2:
                    operand1 = stack->top->operand;
                    rule_tokens[RULE_LENGTH-1-i] = pop(stack);
                    break;
                default:
                    break;
            }

            //abych mel v poli rule_tokens[] tokeny podle komutativity ([0] operand1, [1]operace, [2] operand2;
            //tzn treba [0]2[1]/[2]5 pro vyraz 2/5
            //ve stejnem poradi jsou take operandy v poli operands

            //kontrola, zda se neco kolosalne neposralo. Tohle by nikdy nastat nemelo (Znaci to chybu sava: na stacku
            if(rule_tokens[RULE_LENGTH-1-i]->type == ACTION_MENSITKO || rule_tokens[RULE_LENGTH-1-i]->type == BOTTOM_TOKEN)
            {
                //action_err(stack, sa_vars, ERR_SYN, internal_buffer);
                return false;
            }
        }
    }
    operands[0] = operand3;
    operands[1] = operand2;
    operands[2] = operand1;

    switch (rule)
    {
        //pravidla ve tvaru E->cosi
        //jednoadresna pravidla. Proc pouzivam stack->top?? Protoze to vychazi z definice action_Reduce: redukuju to, oc je na topu
        case 0: //E -> {0,0,INTEGER}, //0
        case 2: //{0,0,FLOAT_2}, //2
        case 3: //{0,0,STRING_1}, //3
        case 5: //{0,0,KEY_NIL},//5
            dest = op_init(rule_tokens[0]->type, savo_name_generator());
            operand1 = op_init(rule_tokens[0]->type, rule_tokens[0]->attribute); //todo pokud nebude fungovat float 2e-1, tak to tady konvertovat.
            tac_defmove_const(sa_vars->tac_list, dest, operand1);
            break;
        case 1: //{0,0,EXPRESSION}, //1 //todo mozna neni potreba - DIVNY
            ;
            dest = operands[0];
            break;
        case 4: //{0, 0,ID_2},//4
            ;
            // item = zaznam o promenne z tokenu z TS
            item = symtab_find(sa_vars->local_tables->top->data,rule_tokens[0]->attribute);
            //vytvarim Toperand, ktery priradim do tokenu Expression
            dest = op_init(item->type, rule_tokens[0]->attribute);
            break;
            //triadresna pravidla
        case 6://{LEFT_BRACKET, INTEGER, RIGHT_BRACKET},//6
        case 8://{LEFT_BRACKET, FLOAT_2, RIGHT_BRACKET},//8
        case 9: //{LEFT_BRACKET, STRING_1, RIGHT_BRACKET},//9
        case 21: //{LEFT_BRACKET, KEY_NIL, RIGHT_BRACKET},//6
            dest = op_init(rule_tokens[1]->type, savo_name_generator());
            operand1 = op_init(rule_tokens[1]->type, rule_tokens[1]->attribute);
            tac_defmove_const(sa_vars->tac_list, dest, operand1);
            break;
        case 7: //{LEFT_BRACKET, EXPRESSION, RIGHT_BRACKET},//7
            ;
            dest = operands[1];
            break;
        case 10: //{LEFT_BRACKET, ID_2, RIGHT_BRACKET},//10
            ;// item = zaznam o promenne z tokenu z TS
            item = symtab_find(sa_vars->local_tables->top->prev->data, rule_tokens[1]->attribute);
            //vytvyrim Toperand, ktery priradim do tokenu Expression
            dest = op_init(item->type, rule_tokens[1]->attribute);
            break;
        case 11:// {EXPRESSION, OP_PLUS, EXPRESSION},//11
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_add(sa_vars->tac_list, dest, operands[2], operands[0]);//TODO prohodit op1a aop2
            break;
        case 12: //{EXPRESSION, OP_MINUS, EXPRESSION},//12
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_sub(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 13: //{EXPRESSION, OP_MULT, EXPRESSION},//13
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_mul(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 14://{EXPRESSION, OP_DIV, EXPRESSION}, //14
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_div(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 15://{EXPRESSION, OP_MORE_1, EXPRESSION}, //15
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_gt(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 16://{EXPRESSION, OP_LESS_1, EXPRESSION},//16
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_lt(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 17://{EXPRESSION, OP_EQAL_1, EXPRESSION},//17
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_eq(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 18://{EXPRESSION, OP_LESS_EQUAL, EXPRESSION},//18
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_lteq(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 19:// {EXPRESSION, OP_MORE_EQUAL, EXPRESSION},//19
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_gteq(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        case 20://{EXPRESSION, OP_NOT_EQ_1, EXPRESSION} //20
            dest = op_init(NOBODY_KNOWS,savo_name_generator());
            tac_defvar(sa_vars->tac_list, dest);
            tac_neq(sa_vars->tac_list, dest, operands[2], operands[0]);
            break;
        default:
            break;//rule == -1
    }

    //for je zde od toho, aby vyhodil ze zasobniku znaky, kterych se tyka pravidlo (ulozeny do internal bufferu davno pred tim)
   /* for (int i = 0; i <RULE_LENGTH; i++) //RULE_LENGHT == 3
    {
        if(rules[rule][i] == 0)
            continue;
        else
        {
            rule_tokens[RULE_LENGTH-1-i] = pop(stack);
            //abych mel v poli rule_tokens[] tokeny podle komutativity ([0] operand1, [1]operace, [2] operand2;
            //tzn treba [0]2[1]/[2]5 pro vyraz 2/5
            //ve stejnem poradi jsou take operandy v poli operands
        }
    }*/
    //overeni, ze ted nasleduje mensitko
    Ttoken *temp = pop(stack); // popnuti mensitka
    if(temp->type != ACTION_MENSITKO) //pokud jsem jako dalsi znak nepopnul mensitko, tak je nekde chyba - u me ne, takze hazim ERR_SEM
        action_err(stack, sa_vars, ERR_SEM_MISC, internal_buffer);
    token_free(temp);
    Ttoken *expr_token = malloc(sizeof(Ttoken));
    if (expr_token == NULL)
        return false;
    token_init(expr_token);
    expr_token->type = EXPRESSION;
    if(rule_tokens[0]->type == KEY_NIL)
        expr_token->attribute = "nil";
    push(stack, stack->top, expr_token, dest);
    //token free trules tokens
    return true;
}

bool savo(TSynCommon *sa_vars)
{
    
    //sa_vars->boolean = true; //todo odstranit, az bude sax funkcni
    int err = 0;  //interni error, pri SYN_ERRORU nepropagovany
    Ttoken *input_token = get_next_token(sa_vars); //token pusnut na buffer az po init bufferu
    if(input_token == NULL) //error handle
    {
        action_err(NULL, sa_vars, ERR_INTERNAL, NULL);
        return false;
    }
    else if(is_terminus(input_token)) //pokud je hned prvni token terminus -> err syntakticky kvuli a = EOL
    {
        action_err(NULL, sa_vars,ERR_SYN, NULL );
        return  false;
    }
    if(input_token->type == FLOAT_2) // cislo
    {
        if(strtof(input_token->attribute, NULL) < 0) //todo Denny by berry. Co mam vracet za err pri spatnem cisle?
        {
            action_err(NULL, sa_vars, ERR_SEM_MISC, NULL);
            return false;
        }
    }
    if(input_token->type == INTEGER && strtol(input_token->attribute, NULL, 10)<0)
    {
        action_err(NULL, sa_vars, ERR_SEM_MISC, NULL);
        return false;
    }
    if(input_token->type == LEX_ERROR)
    {
        action_err(NULL, sa_vars, LEX_ERROR, NULL);
        return false;
    }

    /*Ladici vypis*/
    //fprintf(stderr,"INPUT TOKEN JE: %d\n\n", input_token->type);
    /*Konec ladiciho vypisu*/

    TStack *stack = stack_init();
    if(stack == NULL) //err handle
    {
        action_err(stack,sa_vars,ERR_INTERNAL, NULL);
        return false;

    }
    /*Ladici vypis*/
    //fprintf(stderr, "Stack vytvoren\n");
    /*Konec l.v.*/

    TBuffer *internal_buffer = malloc(sizeof(TBuffer));
    if(internal_buffer == NULL)
    {
        action_err(stack, sa_vars, ERR_INTERNAL, NULL);
        return false;
    }
    buffer_init(internal_buffer);

    buffer_push_top(internal_buffer, input_token);//pushnuti input_token pro jeho nasledne ulozeni.

    Ttoken *stack_token = NULL;

    while(true) //hlavni cast sava - cyklicke provadeni pravidel
    {


        /*Ladici vypis*/
        //fprintf(stderr, "Zacatek hlavniho while cyklu\n");
        /*Konec l.v.*/


        /*UKONCOVACI PODMINKA SAVA*/
        //Pokud jsou oba tokeny(input i stack) vyhodnoceny jako terminus, je cyklus ukoncen. Dale je ukoncen, pokud err != 0
        if(err)
            break;

        stack_token = get_token_from_elem(get_first_terminal(stack)); //stack_token = prvni terminal ze zasobniku

       /*UKONCOVACI PODMINKA SAVA*/
       //Pokud jsou oba tokeny(input i stack) vyhodnoceny jako terminus, je cyklus ukoncen. Dale je ukoncen, pokud err != 0
       if(((stack_token->type == BOTTOM_TOKEN) && is_terminus(input_token)))
            break;

        //pomoc pro sax: osetreni, abych vracel boolean vyrazy jen v situaci, kdy mam
        if(sa_vars->boolean == false)
        {
            if(input_token->type == OP_MORE_1 ||
               input_token->type == OP_LESS_1 ||
               input_token->type == OP_EQAL_1 ||
               input_token->type == OP_LESS_EQUAL ||
               input_token->type == OP_MORE_EQUAL ||
               input_token->type == OP_NOT_EQ_1)
                err = action_err(stack, sa_vars, ERR_SEM_TYPE, internal_buffer);
        }

        /*
         * Popis akci:
         * >: pokud je na zasobniku <XYZ && existuje pravidlo  E -> XYZ,
         * zamen XYZ za E. Jinak se jedna o chybu
         *
         * =: input_token push na zasobnik a vezmi dalsi token ze vstupu
         *
         * <: za prvni terminal od topu zasobniku pushni <, pote pushni input token na vrchol a vezmi dalsi ze vstupu
         *
         * ?: error. Vse dealokuj, interni buffer nahraj do sa_vars a ukonci se s return false,
         *  a mozna err_code
         */
       char action = get_action(input_token, stack_token);

       //kontrola, zda, pokud prisel token s ID_2, je ID_2 v tabulce symbolu. pokud symtab_find == NULL, pak neni => err
        if (input_token->type== ID_2 && (symtab_find(sa_vars->local_tables->top->data, input_token->attribute) == NULL))
        {
            err = action_err(stack, sa_vars, ERR_SEM_DEF, internal_buffer);
            action = 'E';
        }

       switch (action) {
           /*Ladici vypis*/
           //fprintf(stderr, "Rozhodnuti, jaka se provede akce. Momentalne se jedna o: %c \n", action);
           /*Konec l.v.*/
           case '?': //err
               err = action_err(stack, sa_vars, ERR_SYN, internal_buffer);
               break;
           case '<':
               input_token = action_change(input_token, stack, sa_vars, internal_buffer);
               if(input_token == NULL)
                   err = 1;
               break;
           case '>':
               if(!action_reduce(stack, sa_vars, internal_buffer))//todo refaktorovano
               {
                   action_err(stack, sa_vars, ERR_SYN, internal_buffer);
                   err= ERR_SYN;
               }
               break;
           case '=':
               input_token = action_push(input_token, stack, sa_vars, internal_buffer);
               if(input_token == NULL)
                   err = 1;
               break;
            default:
                break;
       }
   }//end while

   if (err != 0)
   {
       return false;
   }
   else
   {
       buffer_push_bottom(sa_vars->buffer, buffer_popTop(internal_buffer));

       //delete_buffer(internal_buffer); //todo dealokovat buffer_elementy. Kvuli saxu musim udelat novou fci :D
      // buffer_init(internal_buffer);
       free(internal_buffer);

       tac_move(sa_vars->tac_list, sa_vars->dest, stack->top->operand);

       delete_stack(stack);
       free(stack);

       return true;
   }
}