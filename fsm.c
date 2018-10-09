//
// Created by danbu on 9.10.2018.
//

#include "fsm.h"
#include <stdbool.h>
int get_token()
{
    int actual_state = START;
    bool final_state = false;

    while(!final_state)
    {
        switch(actual_state)
        {
            case START:
                actual_state = LEX_ERROR;
                break;
            case LEX_ERROR:
                break;
            case IFJ_CODE_PREAM:
                break;
            case OP_PLUS:
                break;
            case OP_MINUS:
                break;
            case OP_DIV:
                break;
            case OP_MULT:
                break;
            case OP_EQAL_0:
                break;
            case OP_EQAL_1:
                break;
            case OP_EQAL_2:
                break;
            case OP_NOT_EQ_0:
                break;
            case OP_NOT_EQ_1:
                break;
            case OP_MORE_0:
                break;
            case OP_MORE_1:
                break;
            case OP_MORE_EQUAL:
                break;
            case OP_LESS_0:
                break;
            case OP_LESS_1:
                break;
            case OP_LESS_EQUAL:
                break;
            case OP_COMMA:
                break;
            case EOL_0:
                break;
            case EOL_1:
                break;
            case BLOCK_COMMENT_0:
                break;
            case BLOCK_COMMENT_1:
                break;
            case BLOCK_COMMENT_2:
                break;
            case BLOCK_COMMMENT_3:
                break;
            case ONE_LINE_COMMENT:
                break;
            case ID_0:
                break;
            case ID_1:
                break;
            case ID_2:
                break;
            case KEY_WORD:
                break;
            case EOF_STATE:
                break;
            case STRING_0:
                break;
            case STRING_1:
                break;
            case ESCAPE_0:
                break;
            case ESCAPE_1:
                break;
            case ESCAPE_2:
                break;
            case NUMBER_0:
                break;
            case INTEGER:
                break;
            case FLOAT_0:
                break;
            case FLOAT_1:
                break;
            case FLOAT_2:
                break;
            case FLOAT_EXP_0:
                break;
            case FLOAT_EXP_1:
                break;
            case FLOAT_EXP_2:
                break;


        }
    }
}