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
*	@author Daniel Bubenicek, Jan Beran
*	@brief implementace konecneho automatu pro lexikalni analyzator
 *	v1.1: Jan Beran: Zacala prace na KA, opraveny chyby a stabni kultura
 *
*/

#include "fsm.h"
#include "err_codes.h"

char TOKEN_TYPE = 'a';//TODO dodelat #define TYPE_INT atd kazdy typ tokenu TOKEN_TYPE pouze docasny

Ttoken get_token(Tarray token_value)
{
    int actual_state = START;
    int next_state = LEX_ERROR;
    bool final_state = false;
    int c;

    while(!final_state)
    {
        switch (actual_state)
        {
            case START: //DONE
                c = get_next_char(&token_value);
                arr_reset(&token_value);
                switch(c)
                {
                    case ' ': //mezera
                    case '\t': //tabulator
                        next_state = START;
                        break;
                    case EOL:
                        next_state = EOL_0;
                        break;
                    case '+':
                        next_state = OP_PLUS;
                        break;
                    case '-':
                        next_state = OP_MINUS;
                        break;
                    case '*':
                        next_state = OP_MULT;
                        break;
                    case '/':
                        next_state = OP_DIV;
                        break;
                    case '(':
                        next_state = LEFT_BRACKET;
                        break;
                    case ')':
                        next_state = RIGHT_BRACKET;
                        break;
                    case '0':
                        next_state = NUMBER_0;
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        next_state = NUMBER_1;
                        break;
                    case '>':
                        next_state = OP_MORE_0;
                        break;
                    case '<':
                        next_state = OP_LESS_0;
                        break;
                    case',':
                        next_state = OP_COMMA;
                        break;
                    case'"':
                        next_state = STRING_0;
                        break;
                    case '#':
                        next_state = ONE_LINE_COMMENT;
                        break;
                    case '.': //.IFJcode18 ??
                    {
                        char ifj[] = ".IFJcode18";
                        for (int i = 0; ifj[i] != '\0'; i++)
                        {
                            if(ifj[i] != c)
                            {
                                next_state = LEX_ERROR;
                                break; //break for
                            }
                            else
                            {
                                arr_add_char(&token_value, c);
                            }
                            c =get_next_char(&token_value);
                        }
                        next_state = IFJ_CODE_PREAM;
                        break; //break case
                    }
                    case EOF:
                        next_state = EOF_STATE;
                        break;
                    default:
                        next_state = LEX_ERROR;
                }
                arr_add_char(&token_value, c);
                break; //konec START
            case LEX_ERROR:
                final_state = true;
                fprintf(stderr, MESSAGE_LEX); //TODO takhle to nepůjde, jak jinak to čistě ukončit??
                break;
            case IFJ_CODE_PREAM: //TODO nezapomenout na token_typ "preambule"
                //TODO sestavit token a poslat ho do syntaktaku
                final_state = true;
                break;
            case OP_PLUS:
                final_state = true;
                break;
            case OP_MINUS:
                final_state = true;
                break;
            case OP_DIV:
                final_state = true;
                break;
            case OP_MULT:
                final_state = true;
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
            case LEFT_BRACKET:
                final_state = true;
                break;
            case RIGHT_BRACKET:
                final_state = true;
                break;
            case OP_COMMA:
                break;
            case EOL_0: //DONE
                c = get_next_char(&token_value);
                if(c == '=')
                    next_state = BLOCK_COMMENT_0;
                else
                {
                    next_state = EOL_1;
                    arr_add_to_buffer(&token_value, c);
                }
                break;
            case EOL_1:
                final_state = true;
                break;
            case BLOCK_COMMENT_0:
                c = get_next_char(&token_value);
                char comm_begin[] = "begin";
                for (int i = 0; comm_begin[i] != '\0'; i++)
                {
                    if(comm_begin[i] != c)
                    {
                        next_state = LEX_ERROR;
                        break; //break for
                    }
                    else
                    {
                        arr_add_char(&token_value, c);
                    }
                    c = get_next_char(&token_value);
                }
                if (c == ' ' || c == '\t')
                {
                    next_state = BLOCK_COMMENT_1;
                    arr_add_to_buffer(&token_value, c);
                }
                else
                    next_state = LEX_ERROR;
                break;
            case BLOCK_COMMENT_1:
                c = get_next_char(&token_value);
                while(c != EOL && c != EOF)
                {
                    c = get_next_char(&token_value);
                }
                if(c == EOL)
                    next_state = BLOCK_COMMENT_2;
                else
                    next_state = LEX_ERROR;
                break;
            case BLOCK_COMMENT_2:
                c = get_next_char(&token_value);
                char comm_end[] = "=end";
                for (int i = 0; comm_end[i] != '\0'; i++)
                {
                    if(comm_end[i] != c)
                    {
                        next_state = LEX_ERROR;
                        break; //break for
                    }
                    else
                    {
                        arr_add_char(&token_value, c);
                    }
                    c = get_next_char(&token_value);
                }
                if (c == ' ' || c == '\t')
                {
                    next_state = START;
                    arr_add_to_buffer(&token_value, c);
                }
                else
                    next_state = BLOCK_COMMENT_1;
                break;
            case BLOCK_COMMMENT_3: //TODO vyhodit?
                break;
            case ONE_LINE_COMMENT:
                c = get_next_char(&token_value);
                while(c != EOL && c != EOF)
                {
                    c = get_next_char(&token_value);
                }
                if(c == EOF)
                    next_state = EOF_STATE;
                if(c == EOL)
                    next_state = START;
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
                final_state = true;
                break;
            case STRING_0:
                c = get_next_char(&token_value);
                while (c != '"' && c != '\\')
                {
                    arr_add_char(&token_value, c);
                    c = get_next_char(&token_value);
                }
                if (c == '"')
                    next_state = STRING_1;
                else if (c == '\\')
                    next_state = ESCAPE_0;
                break;
            case STRING_1:
                final_state = true;
                break;
            case ESCAPE_0:
                c = get_next_char(&token_value); //nactu znak bez lomitka
                switch(c)
                {
                    case '"':
                        arr_add_char(&token_value, '\"');
                        break;
                    case 'n':
                        arr_add_char(&token_value, '\n');
                        break;
                    case 't':
                        arr_add_char(&token_value, '\t');
                        break;
                    case 's':
                        arr_add_char(&token_value, ' ');
                        break;
                    case '\\':
                        arr_add_char(&token_value, '\\');
                        break;
                    case 'x':
                        next_state = ESCAPE_1;
                        break;
                    default:
                        arr_add_char(&token_value, c);
                        next_state = STRING_1;
                        break;
                }
                break;
            case ESCAPE_1:
                c = get_next_char(&token_value);
                char hexa[2];
                int num = 0;
                if((c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'f') ||
                   (c >= 'A' && c <= 'F'))    //pokud je c 0..9a..fA..F
                {
                    hexa[0]=c;
                    c = get_next_char(&token_value);
                    if((c >= '0' && c <= '9') ||
                       (c >= 'a' && c <= 'f') ||
                       (c >= 'A' && c <= 'F'))    //pokud je c 0..9a..fA..F
                    {
                        hexa[1] = c;
                        //TODO - Je zadano char pole o dvou znacích, které vyjadřuje číslo v hexasoustavě. Cíl: funkce, která vrátí znak s danou hodnotou v ASCII tabulce.
                        sscanf(hexa, "%x", &num);
                        arr_add_char(&token_value, (char)c);
                    }
                }

                break;
            case ESCAPE_2: // asi pryč
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
            default:
                next_state = ERR_INTERNAL;
        }
        actual_state = next_state;
    }
}

int arr_init(Tarray *arr)
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

int get_from_buffer(Tarray *arr)
{
    arr->buffer_flag = false;
    return arr->buffer;
}

int get_next_char(Tarray *arr)
{
  if(arr->buffer_flag == true)
      return get_from_buffer(arr);
  else
      return getchar();
}
int arr_add_to_buffer(Tarray *arr, char c)
{
    if(arr == NULL) return ERR_INTERNAL;
    arr->buffer = c;
    arr->buffer_flag = true;
    return SUCCESS;
}
int arr_reset(Tarray *arr)
{
    if(arr->lenght != INIT_SIZE) //pokud je pole vetsi nez vychozi velikost
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
    //TODO DENNY, jsi si jist prioritou operaci? nemaji tam byt zavorky?
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


//funkce pro praci s tokenem
int token_init(Ttoken *token)
{
    token->type = (char *) malloc(sizeof(char) * INIT_SIZE); //typ by INIT_SIZE presahnout nikdy nemel
    if(token->type == NULL)
        return ERR_INTERNAL;

    token->attribute = (char *) malloc (sizeof(char) * INIT_SIZE); //atribut bude muset byt realokovan dle velikosti pole arr
    if(token->attribute == NULL)
    {
        free(token->type);
        return ERR_INTERNAL;
    }
    token->a_len = INIT_SIZE;
    token->a_used = 0;
    token->t_used = 0;
        return SUCCESS;
}

char *token_get_type(Ttoken *token)
{
    char *output = (char *) malloc(sizeof(char)*(token->t_used +1));//alokace pro predavany retezec, jedno misto navic pro \0
    if(output == NULL)
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        return NULL;
    }
    for(int i = 0; i < token->t_used; i++) //kopirovani pole
        output[i] = token->type[i];
    output[token->t_used] = '\0'; //na konec retezce dam ukoncovaci znak
    return output;
}


char *token_get_attribute(Ttoken *token)
{
    char *output = (char *) malloc(sizeof(char)*(token->a_used +1));//alokace pro predavany retezec, jedno misto navic pro \0
    if(output == NULL)
    {
        fprintf(stderr, MESSAGE_ALLOCATION);
        return NULL;
    }
    for(int i = 0; i < token->a_used; i++) //kopirovani pole
        output[i] = token->attribute[i];
    output[token->a_used] = '\0'; //na konec retezce dam ukoncovaci znak
    return output;
}


int token_load_type(Ttoken *token, char *token_type)
{
    if(token->type == NULL)
        return ERR_INTERNAL;
    for(int i =0; token_type[i] != '\0'; i++) // kopirovani typu tokenu do token->type
    {
        token->type[i] = token_type[i];
        token->t_used += 1;
    }
    return SUCCESS;
}

int token_load_attribute(Ttoken *token, Tarray *arr)
{
    if(token->a_len <= arr->used) //pokud je maximalni delka atributu mensi nebo rovna nez arr->used  realokovat
    {
        char *temp_ptr = (char *) realloc(token->attribute, sizeof(char) * token->a_len * 2); //zvetseni pole na dvojnasobek, stejne jako u arr
        if(temp_ptr == NULL) //realokace neuspesna
        {
            fprintf(stderr, MESSAGE_ALLOCATION);
            free(token->attribute); //dealokace pole
            return ERR_INTERNAL;
        }
        token->attribute = temp_ptr; //predani noveho ukazatele na pole
        token->a_len *= 2; //zdvojnasobena velikost pole
    }
    for(int i = 0; i < arr->used; i++) //kopirujeme z arr->array do token->attribute
    {
        token->attribute[i] = arr->array[i];
        token->a_used += 1;
    }
}

void token_free(Ttoken *token)
{
    free(token->type);
    free(token->attribute);
}