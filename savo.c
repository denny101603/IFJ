//
// Created by janbe on 08.11.2018.
//
#include "savo.h"

char get_action(Ttoken input_token, Ttoken stack_token)
{
    int stack_terminal =-1;
    int input_terminal = -1;
    /* + 	  -   *	  /	  (	  )	 id	 con  <   >  <=	  >=  !=  ==   $ */
    switch(input_token.type)
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
            stack_terminal = 7;
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
        case TERMINUS:
            input_terminal = 14;
            break;
    }
    switch (stack_token.type)
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
        case TERMINUS:
            stack_terminal = 14;
            break;
    }
    if (input_terminal == -1 || stack_terminal == -1)
        return '?'; //neni zaznam v tabulce
    return prec_table[stack_terminal][input_terminal];
}

bool is_terminus(Ttoken token)
{
    if(token.type == EOF_STATE ||
       token.type == EOL_1 ||
       token.type == OP_COMMA ||
       token.type == KEY_THEN ||
       token.type == KEY_DO ||
       token.type == RIGHT_BRACKET
       )
        return true;
    return false;
}

bool stack_init(TStack *stack)
{
    TStackElem *top = (TStackElem *) malloc(sizeof(TStackElem));
    if(top == NULL)
    {
        return false;
    }
    Ttoken *first = (Ttoken *)malloc(sizeof(Ttoken));
    if(first == NULL)
    {
        return false;
    }
    first->type = BOTTOM_TOKEN;
    top->data = first;
    top->next = NULL;
    top->prev = NULL;
    stack->top = top;
    return true;
}

bool push(TStack *stack, Ttoken *token)
{
    TStackElem *top = (TStackElem *) malloc(sizeof(TStackElem));
    if(top == NULL)
    {
        return false;
    }
    top->data = token;

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
    TStackElem *temp = stack->top;
    while (temp->data->type == EXPRESSION)
    {
        temp = temp->prev;
    }
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
    return;
}

Ttoken action_push(Ttoken input_token, TStack *stack)
{
    push(stack, &input_token);
    Tarray arr;
    arr_init(&arr); //Pole znaku
    return get_token(&arr);
}

Ttoken action_change(Ttoken input_token, TStack *stack)
{
    Ttoken mensitko;
    mensitko.type = ACTION_MENSITKO;
    push(stack, &mensitko);
    return action_push(input_token, stack);
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
    Ttoken *expr_token = NULL;
    token_init(expr_token);
    expr_token->type = EXPRESSION;
    push(stack, expr_token);
}

bool savo(Ttoken *input_token)
{
    int err = 0;
    TStack stack;
    stack_init(&stack);
    while(true)
    {
       Ttoken *stack_token = get_first_terminal(&stack);
       /*UKONCOVACI PODMINKA SAVA*/
       //Pokud jsou oba tokeny(input i stack) vyhodnoceny jako terminus, je cyklus ukoncen
       if((stack_token->type == BOTTOM_TOKEN) && is_terminus(*input_token))
           break;
       char action = get_action(*input_token, *stack_token);
       switch (action) {
           case '?':
               err = action_err(&stack);
               break;
           case '<':
               action_change(*input_token, &stack);
               break;
           case '>':
               action_reduce(&stack);
               break;
           case '=':
               action_push(*input_token, &stack);
               break;
       };
   }
}