//
// Created by janbe on 08.11.2018.
//
#include "savo.h"


/**
 * @brief Tabulka pravidel pouzivana funkci get_action.
 * @author Matej Jelinek, Jan Beran
 */
int rules[NUM_OF_RULES][RULE_LENGTH] = {
        {EXPRESSION, 0,0},
        {EXPRESSION, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, EXPRESSION},
        {LEFT_BRACKET, EXPRESSION, RIGHT_BRACKET},
        {ID_2, 0, 0},
        {EXPRESSION, OP_PLUS, ID_2},
        {ID_2, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, ID_2},
        {ID_2, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, ID_2},
        {ID_2, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, ID_2},
        {ID_2, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, ID_2},
        {ID_2, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, ID_2},
        {ID_2, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, ID_2},
        {ID_2, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, ID_2},
        {ID_2, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, ID_2},
        {ID_2, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, ID_2},
        {ID_2, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, INTEGER},
        {INTEGER, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, INTEGER},
        {INTEGER, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, INTEGER},
        {INTEGER, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, INTEGER},
        {INTEGER, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, INTEGER},
        {INTEGER, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, INTEGER},
        {INTEGER, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, INTEGER},
        {INTEGER, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, INTEGER},
        {INTEGER, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, INTEGER},
        {INTEGER, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, INTEGER},
        {INTEGER, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, FLOAT_2},
        {FLOAT_2, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MINUS, FLOAT_2},
        {FLOAT_2, OP_MINUS, EXPRESSION},
        {EXPRESSION, OP_MULT, FLOAT_2},
        {FLOAT_2, OP_MULT, EXPRESSION},
        {EXPRESSION, OP_DIV, FLOAT_2},
        {FLOAT_2, OP_DIV, EXPRESSION},
        {EXPRESSION, OP_MORE_1, FLOAT_2},
        {FLOAT_2, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, FLOAT_2},
        {FLOAT_2, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, FLOAT_2},
        {FLOAT_2, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, FLOAT_2},
        {FLOAT_2, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, FLOAT_2},
        {FLOAT_2, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, FLOAT_2},
        {FLOAT_2, OP_NOT_EQ_1, EXPRESSION},
        {EXPRESSION, OP_PLUS, STRING_1},
        {STRING_1, OP_PLUS, EXPRESSION},
        {EXPRESSION, OP_MORE_1, STRING_1},
        {STRING_1, OP_MORE_1, EXPRESSION},
        {EXPRESSION, OP_LESS_1, STRING_1},
        {STRING_1, OP_LESS_1, EXPRESSION},
        {EXPRESSION, OP_EQAL_2, STRING_1},
        {STRING_1, OP_EQAL_2, EXPRESSION},
        {EXPRESSION, OP_LESS_EQUAL, STRING_1},
        {STRING_1, OP_LESS_EQUAL, EXPRESSION},
        {EXPRESSION, OP_MORE_EQUAL, STRING_1},
        {STRING_1, OP_MORE_EQUAL, EXPRESSION},
        {EXPRESSION, OP_NOT_EQ_1, STRING_1},
        {STRING_1, OP_NOT_EQ_1, EXPRESSION},
};

/**
 * @brief Precedencni tbaulka pro syntaktickou analyzu vyrazu.
 * @author Jan Carba, Jan Beran
 */
char prec_table[15][15] = {
        /*           + 	  -   *	  /	  (	  )	 id	 con  <   >  <=	  >=  !=  ==   $ */
        /* + */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* - */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* * */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* / */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* ( */    {'<','<','<','<','<','=','<','<','<','<','<','<','<','<','?'},
        /* ) */    {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* id */   {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* cons */ {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* < */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* > */    {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* <= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* >= */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* != */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* == */   {'<','<','<','<','<','>','<','<','?','?','?','?','?','?','>'},
        /* $ */    {'<','<','<','<','<','?','<','<','?','?','?','?','?','?','?'}
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
            input_terminal = 7;
            break;
        case FLOAT_2:
            input_terminal = 7;
            break;
        case OP_LESS_1:
            input_terminal = 8;
            break;
        case OP_MORE_1:
            input_terminal = 9;
            break;
        case OP_LESS_EQUAL:
            input_terminal = 10;
            break;
        case OP_MORE_EQUAL:
            input_terminal = 11;
            break;
        case OP_NOT_EQ_1:
            input_terminal = 12;
            break;
        case OP_EQAL_2:
            input_terminal = 13;
            break;
        case EOF_STATE: //upravovano
        case EOL_1:
        case OP_COMMA:
        case KEY_THEN:
        case KEY_DO:
            input_terminal = 14;
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
            stack_terminal = 7;
            break;
        case FLOAT_2:
            stack_terminal = 7;
            break;
        case OP_LESS_1:
            stack_terminal = 8;
            break;
        case OP_MORE_1:
            stack_terminal = 9;
            break;
        case OP_LESS_EQUAL:
            stack_terminal = 10;
            break;
        case OP_MORE_EQUAL:
            stack_terminal = 11;
            break;
        case OP_NOT_EQ_1:
            stack_terminal = 12;
            break;
        case OP_EQAL_2:
            stack_terminal = 13;
            break;
        case BOTTOM_TOKEN:
            stack_terminal = 14;
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
    fprintf(stderr,"init token");
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
    fprintf(stderr,"token je: %d \n", stack->top->data->type);
    return stack;
}

bool push(TStack *stack, Ttoken *token)
{
    TStackElem *top = (TStackElem *) malloc(sizeof(TStackElem));
    if(top == NULL)
    {
        return false;
    }
    top->data = token;
    top->next = NULL;
    top->prev = stack->top;
    stack->top->next = top;
    stack->top = top;
    //printf("test: %d %d\n",stack->top->data->type, token->type);
    return true;
}

Ttoken *pop(TStack *stack)
{
    TStackElem *temp = stack->top;
    stack->top = stack->top->prev;
    stack->top->next = NULL;
    Ttoken *output = temp->data;
    free(temp);
    return output;
}

Ttoken *get_first_terminal(TStack *stack)
{

    fprintf(stderr, "jsme ve fci get first terminal.\n");
    TStackElem *temp = stack->top;
    while (temp->data->type == EXPRESSION)
    {
        temp = temp->prev;
    }
    fprintf(stderr, "koncim terminal\n");
    return temp->data;
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
// TODO 18.56
Ttoken *action_push(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars)
{
    push(stack, input_token);
    return get_next_token(sa_vars->arr, sa_vars->buffer);
}

Ttoken *action_change(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars)
{
    Ttoken *mensitko = malloc(sizeof(Ttoken));
    if(mensitko == NULL)
        return NULL;
    mensitko->type = ACTION_MENSITKO;
    //push(stack, mensitko);
    return action_push(input_token, stack, sa_vars);
}

bool action_reduce(TStack *stack)
{
    int rule = find_rule(stack);
    if(rule != -1)
        execute_rule(rule, stack);
    else
        return false;
    return true;
}

int action_err(TStack *stack)
{
    delete_stack(stack);
    return ERR_SYN;
}
//todo 18.58
int find_rule(TStack *stack)
{
    int rule[3] = {0,0,0};
    int ret = -1;
    TStackElem *temp = stack->top;
    for(int index = 2; index > -1 || temp->data->type != ACTION_MENSITKO; index-- )
    {
        rule[index] = temp->data->type;
        temp = temp->prev;
    }

        for( int i = 0; i < NUM_OF_RULES; i++)
        {
            for(int j = 0; j < RULE_LENGTH; j++)
            {
                if (rule[j] != rules[i][j])
                    break;
                else if (rule[j] == rules[i][j] && j == RULE_LENGTH-1) // cele pravidlo se rovna
                    ret = i;
            }
        }
    return ret;

}

void execute_rule(int rule, TStack *stack)
{
    for (int i = 2; i  > RULE_LENGTH ; i--)
    {
        if(rules[rule][i] == 0) //napr 1,0,0 (jdeme odzadu)
            continue;
        else
            pop(stack);
    }
    pop(stack);
    Ttoken *expr_token = malloc(sizeof(Ttoken));
    if (expr_token == NULL)
        return;
    token_init(expr_token);
    expr_token->type = EXPRESSION; //TODO by berry muze byt expr null?
    push(stack, expr_token);
}

bool savo(TSynCommon *sa_vars)
{
    Ttoken *input_token = get_next_token(sa_vars->arr, sa_vars->buffer);
    int err = 0;
    TStack *stack = stack_init();

    fprintf(stderr, "savo, stack inited\n");
    while(true)
    {
        if(stack == NULL)
            fprintf(stderr, "stack je NULL :( \n");
        fprintf(stderr, "na zacatku while, input_token je %d, na stacku by mel byt dolar %d \n", input_token->type, stack->top->data->type );
        fflush(stderr);
        Ttoken *stack_token = get_first_terminal(stack);
        fprintf(stderr, "terminalovano\n");
       /*UKONCOVACI PODMINKA SAVA*/
       //Pokud jsou oba tokeny(input i stack) vyhodnoceny jako terminus, je cyklus ukoncen. Dale je ukoncen, pokud err != 0
       if(((stack_token->type == BOTTOM_TOKEN) && is_terminus(input_token)) || err != 0)
           break;

       char action = get_action(input_token, stack_token);
       switch (action) {
           default:
           case '?': //err
               fprintf(stderr, "err %d %d\n", input_token->type, stack->top->data->type);
               err = action_err(stack);
               break;
           case '<': //na zasobnik dej <, pushni token a vezmi si dalsi
               fprintf(stderr, "na zasobnik dej <, pushni token a vezmi si dalsi %c \n", input_token->type);
               input_token = action_change(input_token, stack, sa_vars);
               break;
           case '>': //pokud je na vrcholu zasobniku <XYZ, tak to popni a zahod >
               fprintf(stderr, "pokud je na vrcholu zasobniku <XYZ, tak to popni a zahod > %c \n", input_token->type);
               action_reduce(stack);
               break;
           case '=': //push token a nacti dalsi
               fprintf(stderr, "push token a nacti dalsi %c \n", input_token->type);
               input_token = action_push(input_token, stack, sa_vars);
               break;
       }
   }//end while
   if (err != 0) //TODO berry  handle error
   {
       return false;
   }
   else
       return true;
}