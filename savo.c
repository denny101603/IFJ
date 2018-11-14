//
// Created by janbe on 08.11.2018.
//
#include "savo.h"

#define NUM_OF_RULES 88
#define RULE_LENGTH 3

/**
 * @brief Tabulka pravidel pouzivana funkci get_action.
 * @author Matej Jelinek, Jan Beran
 */
int rules[NUM_OF_RULES][RULE_LENGTH] = {
        {0,0,INTEGER},
        {0,0,EXPRESSION},
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
        {0, 0,ID_2},
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
char prec_table[15][15] = {              //in
        /*           + 	  -   *	  /	  (	  )	 id	 con  <   >  <=	  >=  !=  ==   $ */
        /* + */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* - */    {'>','>','<','<','<','>','<','<','>','>','>','>','>','>','>'},
        /* * */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* / */    {'>','>','>','>','<','>','<','<','>','>','>','>','>','>','>'},
        /* ( */    {'<','<','<','<','<','=','<','<','<','<','<','<','<','<','?'},
        /* ) */    {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
        /* id */   {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
/*stack    cons */ {'>','>','>','>','?','>','?','?','>','>','>','>','>','>','>'},
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

bool push(TStack *stack, TStackElem *stack_elem, Ttoken *input_token)
{
    TStackElem *inputed = (TStackElem *) malloc(sizeof(TStackElem));
    if(inputed == NULL)
    {
        return false;
    }

    inputed->data = input_token;
    inputed->next = stack_elem->next;
    inputed->prev = stack_elem;

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
    TStackElem *temp = stack->top;
    stack->top = stack->top->prev;
    stack->top->next = NULL;
    Ttoken *output = temp->data;
    free(temp);
    return output;
}

bool copy_buffer(TBuffer *src, TBuffer *dst)
{
    if(src == NULL)
        return true;
    Ttoken *tempsrc = NULL;
    while(src!=NULL) //updated via poptop
    {
        tempsrc = buffer_popTop(src);
        buffer_push_bottom(dst, tempsrc);
    }
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

Ttoken *action_push(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars)
{
    push(stack, get_first_terminal(stack),input_token);
    Ttoken *ret = get_next_token(sa_vars->arr, sa_vars->buffer);
    //fprintf(stderr,"\n\n*******INPUT TOKEN JE: %d\n\n", ret->type);
    return ret;
}

Ttoken *action_change(Ttoken *input_token, TStack *stack, TSynCommon *sa_vars)
{
    Ttoken *mensitko = malloc(sizeof(Ttoken));
    if(mensitko == NULL)
        return NULL;
    mensitko->type = ACTION_MENSITKO;
    push(stack,get_first_terminal(stack), mensitko); //pushneme mensitko za prvni terminal
    push(stack, stack->top, input_token);
    Ttoken *ret = get_next_token(sa_vars->arr, sa_vars->buffer);
    return ret;
}

bool action_reduce(TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    int rule = find_rule(stack);
    if(rule != -1)
        execute_rule(rule, stack, sa_vars, internal_buffer);
    else
        return false;
    return true;
}

int action_err(TStack *stack, TSynCommon *sa_vars, int error, TBuffer *internal_buffer)
{
    if (error != ERR_SYN) //ERR_SYN je osetrovan pouhym vracenim false; sax obcas dava savu i nerelevantni vstup s cilem zjistit, jestli to je vyraz. Toto nema zpusobit pad
        sa_vars->err_code = error;
    if (stack != NULL)
        delete_stack(stack);
    copy_buffer(internal_buffer, sa_vars->buffer); //presunuti interniho bufferu do spolecneho se sax

    return error;
}

int find_rule(TStack *stack)
{
    int rule[3] = {0,0,0};
    int ret = -1;
    //fprintf(stderr,"find rule, %d \n", stack->top->data->type);
    TStackElem *temp = stack->top;
    for(int index = 2; index > -1 && temp->data->type != ACTION_MENSITKO; index-- )
    {
        rule[index] = temp->data->type;
        if(temp->prev != NULL)
            temp = temp->prev;
    }
    //pop(stack); // popnuti mensitka

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
void execute_rule(int rule, TStack *stack, TSynCommon *sa_vars, TBuffer *internal_buffer)
{
    for (int i = 0; i <RULE_LENGTH; i++)
    {
        if(rules[rule][i] == 0) //napr 1,0,0 (jdeme odzadu)
            continue;
        else
        {
            Ttoken *temp = pop(stack);
            if(!is_pseudotoken(temp))
                buffer_push_top(internal_buffer, temp);
        }

    }
    Ttoken *temp = pop(stack);
    if(!is_pseudotoken(temp))
        buffer_push_top(internal_buffer, temp);
    Ttoken *expr_token = malloc(sizeof(Ttoken));
    if (expr_token == NULL)
        return;
    token_init(expr_token);
    expr_token->type = EXPRESSION; //TODO by berry muze byt expr null?
    push(stack, get_first_terminal(stack), expr_token); //TODO berry nema tam byt stack->top misto get_first terminal?? ja ma pocit, že ano
}

bool savo(TSynCommon *sa_vars)
{
    Ttoken *input_token = get_next_token(sa_vars->arr, sa_vars->buffer);
    if(input_token == NULL) //error handle
    {
        action_err(NULL, sa_vars, ERR_INTERNAL, NULL);
        return false;
    }
    /**///fprintf(stderr,"\n\n*******INPUT TOKEN JE: %d\n\n", input_token->type);
    int err = 0;

    TStack *stack = stack_init();
    if(stack == NULL) //err handle
    {
        action_err(stack,sa_vars,ERR_INTERNAL, NULL);
        return false;

    }//fprintf(stderr, "savo, stack inited\n");
    TBuffer *internal_buffer = malloc(sizeof(TBuffer));
    if(internal_buffer == NULL)
    {
        action_err(stack, sa_vars, ERR_INTERNAL, NULL);
        return false;
    }
    buffer_init(internal_buffer);

    while(true)
    {
        if(err) break;
        /**///fprintf(stderr, "na zacatku while, input_token je %d, na stacku %d \n", input_token->type, stack->top->data->type );
        /**/fflush(stderr);
        Ttoken *stack_token = get_token_from_elem(get_first_terminal(stack));
        /**///fprintf(stderr, "terminalovano %d %d \n", input_token->type, stack_token->type);

       /*UKONCOVACI PODMINKA SAVA*/
       //Pokud jsou oba tokeny(input i stack) vyhodnoceny jako terminus, je cyklus ukoncen. Dale je ukoncen, pokud err != 0
       if(((stack_token->type == BOTTOM_TOKEN) && is_terminus(input_token)))
           break;

       char action = get_action(input_token, stack_token);
       switch (action) {
           default:
           case '?': //err
               /**///fprintf(stderr, "err in:%d stack:%d\n", input_token->type, stack_token->type);
               err = action_err(stack, sa_vars, ERR_SYN, internal_buffer);
               break;
           case '<': //na zasobnik dej <, pushni token a vezmi si dalsi
               //fprintf(stderr, "na zasobnik dej <, pushni token a vezmi si dalsi %c \n", input_token->type);
               input_token = action_change(input_token, stack, sa_vars);
               break;
           case '>': //pokud je na vrcholu zasobniku <XYZ, tak to popni a zahod >
               //fprintf(stderr, "pokud je na vrcholu zasobniku <XYZ, tak to popni a zahod > %c \n", input_token->type);
               if(!action_reduce(stack, sa_vars, internal_buffer))
               {
                   action_err(stack, sa_vars, ERR_SYN, internal_buffer);
                   err= ERR_SYN;
               }
               break;
           case '=': //push token a nacti dalsi
               //fprintf(stderr, "push token a nacti dalsi %c \n", input_token->type);
               input_token = action_push(input_token, stack, sa_vars);
               //fprintf(stderr,"\n\n*******INPUT TOKEN JE: %d\n\n", input_token->type);
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