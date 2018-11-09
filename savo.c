//
// Created by janbe on 08.11.2018.
//
#include "savo.h"


char get_action(Ttoken input_token, Ttoken stack_token)
{
    int stack_nonterminal = -1;
    int input_terminal = -1;
    //TODO pridelit kazdemu moznemu typu tokenu cislo, odpovidajici radku/sloupci v prec. tabulce
    //Pr: tokenu typu "-" odpovida radek/sloupec cislo 1 (radek, pokud to je token ze stacku, sloupec, pokud to je token ze vstupu)
    //TODO pokud

    if (input_terminal == -1 || stack_nonterminal == -1)
        return '?'; //neni zaznam v tabulce
    return prec_table[stack_nonterminal][input_terminal];
}