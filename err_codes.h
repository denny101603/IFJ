/*
 * err_codes.h jako soucast projektu do IFJ 2018
 * Hlavickovy soubor obsahuje vsechny potrebne chybove kody vcetne ERR_OK - navrat bez chyby.
 * Autor: xberan43
 * Verze: 1.1
 * Datum: 28.9.2018
 *
 */

#ifndef IFJ2018_ERR_CODES_H
#define IFJ2018_ERR_CODES_H

#define ERR_OK 0 //program skoncil bez chyb
#define ERR_LEX 1 // chyba lexikalni analyzy – chybná struktura aktuálního lexému
#define ERR_SYN 2 //chyba syntaktické analýzy – chybná syntaxe
#define ERR_SEM_DEF 3 //chybna semantika programu – nedefinovana fce/prom, redefinice fce/prom atp.
#define ERR_SEM_TYPE 4 //chyba vramci typove kompatibility ve vyrazech
#define ERR_SEM_PARAM 5 //chybny pocet parametru pri volani fce
#define ERR_SEM_MISC 6 //ostatni semanticke chyby
#define ERR_ZERO_DIVISION 9 //deleni nulou
#define ERR_INTERNAL 99 //interni chyba programu nezavisla na vstupu – chyba alokace pameti, neco se nam nepovedlo atp...

#endif //IFJ2018_ERR_CODES_H
