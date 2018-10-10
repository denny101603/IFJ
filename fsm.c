/***************************************
* 	IFJ projekt 2018                   *
* 						               *
*	Autori:			                   *
*	Jan Beran (xberan43)	           *
*	Daniel Bubenicek (xbuben43)	       *
*	Jan Carba (xcarba00)		       *
*	Matej Jelinek (xjelen49)	       *
*                                      *
***************************************/
/**
*	@file fsm.c
*	@author Daniel Bubenicek
*	@brief implementace konecneho automatu pro lexikalni analyzator
*/

#include "fsm.h"
#include "err_codes.h"

int get_token()
{
    int actual_state = START;
    bool final_state = false;

    while(!final_state) {
        switch (actual_state) {
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
int init_array(Tarray *arr)
{
    arr->array = (char *) malloc(sizeof(char) * INIT_SIZE);
    if(arr->array == NULL)
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        return ERR_INTERNAL;
    }
    arr->lenght = INIT_SIZE;
    arr->used = 0;
    arr->buffer_flag = false;
    arr->eol_flag = false;
    return SUCCESS;
}

int arr_add_char(Tarray *arr, char c)
{
    if(arr->lenght > arr->used) //pokud je misto
    {
        arr->array[arr->used++] = c; //pridani prvku na prvni volne misto a zvyseni used
        return SUCCESS;
    }
    char *temp_ptr = (char *) realloc(arr->array, sizeof(char) * arr->lenght * 2); //zvetseni pole na dvojnasobek
    if(temp_ptr == NULL) //realokace neuspesna
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        free(arr->array); //dealokace pole
        return ERR_INTERNAL;
    }
    arr->array = temp_ptr; //predani noveho ukazatele na pole
    arr->lenght *= 2; //velikost se nam zdvojnasobila
    arr->array[arr->used++] = c; //pridani prvku na prvni volne misto a zvyseni used
    return SUCCESS;
}

int arr_reset(Tarray *arr)
{
    if(arr->lenght != INIT_SIZE) //pokud je pole vetsi nez vychozi velikos
    {
        char *temp_ptr = (char *) realloc(arr->array, sizeof(char) * INIT_SIZE); //zmenseni na INIT_SIZE
        if(temp_ptr == NULL) //realokace neuspesna
        {
            fprintf(stderr, MESSAGE_ALLOCATION);
            free(arr->array); //dealokace pole
            return ERR_INTERNAL;
        }
        arr->array = temp_ptr; //predani noveho ukazatele na pole
        arr->lenght = INIT_SIZE;
    }
    arr->used = 0;
    return SUCCESS;
}

void arr_free(Tarray *arr)
{
    free(arr->array);
}

char *arr_get_value(Tarray *arr)
{
    char *output = (char *) malloc(sizeof(char) * arr->used + 1); //alokace pro predavany retezec, jedno misto navic pro \0
    if(output == NULL)
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        return NULL;
    }
    for(int i = 0; i < arr->used; i++) //kopirovani pole
        output[i] = arr->array[i];
    output[arr->used] = '\0'; //na konec retezce dam ukoncovaci znak
    return output;
}