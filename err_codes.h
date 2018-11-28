/**
 * err_codes.h jako soucast projektu do IFJ 2018
 * Hlavickovy soubor obsahuje vsechny potrebne chybove kody vcetne ERR_OK - navrat bez chyby.
 * Autor: Jan Beran
 * Verze: 1.2
 * Datum: 11.10.2018
 * v1.2: doplněna chybová hlášení
 */
//
#ifndef IFJ2018_ERR_CODES_H
#define IFJ2018_ERR_CODES_H

#define IN_PROGRESS -1 //SA stale probiha

#define SUCCESS 0 //program skoncil bez chyb
#define ERR_LEX 1 // chyba lexikalni analyzy – chybná struktura aktuálního lexému
#define ERR_SYN 2 //chyba syntaktické analýzy – chybná syntaxe
#define ERR_SEM_DEF 3 //chybna semantika programu – nedefinovana fce/prom, redefinice fce/prom atp.
#define ERR_SEM_TYPE 4 //chyba vramci typove kompatibility ve vyrazech
#define ERR_SEM_PARAM 5 //chybny pocet parametru pri volani fce
#define ERR_SEM_MISC 6 //ostatni semanticke chyby
#define ERR_ZERO_DIVISION 9 //deleni nulou
#define ERR_INTERNAL 99 //interni chyba programu nezavisla na vstupu – chyba alokace pameti, neco se nam nepovedlo atp...

#define	MESSAGE_LEX "Chyba v lexikalnim analyzatoru, program bude ukoncen.\n"
#define	MESSAGE_SYN "Chyba syntakticke analyzy, program bude ukoncen.\n"
#define MESSAGE_SEM_DEF "Semanticka chyba (problem s definici). Program bude ukoncen.\n"
#define MESSAGE_SEM_TYPE "Semanticka chyba (typova kompatibilita). Program bude ukoncen.\n"
#define	MESSAGE_SEM_PARAM "Volani funkce s nespravnym poctem parametru, program bude ukoncen.\n"
#define MESSAGE_SEM_MISC "Doslo k neidentifikovane semanticke chybe. Program bude ukoncen.\n"
#define MESSAGE_ZERO_DIVISION "Doslo k deleni nulou. Vesmir bude znicen za 3,2,1...\n"
#define	MESSAGE_INTERNAL_ERROR "Doslo k nespecifikovane interni chybe programu a ten bude nyni ukoncen.\n"
#define MESSAGE_ALLOCATION "Doslo k chybe pri alokaci pameti. Program bude nyni ukoncen.\n"

#endif //IFJ2018_ERR_CODES_H
/**
 * Recept na cokoladove muffiny
 * @author Jan Beran :)
 *
 * @Suroviny
 * 200 g hladke mouky
 * 50 g polohrube mouky
 * 110 g cukru krupice (staci mene)
 * 2 lzice holandskeho kakaa (doporucuji vice)
 * pytlik prasku do peciva
 * 2 tabulky nasekane cokolady (doporucuji horkou (nad r patri carka :) ))
 * 2 vejce
 * 250 g jogurtu
 * 85 g masla
 * lzicka vanilky
 *
 * @Postup
 * Sypke suroviny smichame dohromady.
 * Pote prilijeme ostatni (tekute) suroviny a vzniklou smes naplnime do misek na muffiny(nejlepe skoro k okraji).
 * Peceme cca 25 minut na 180 °C.
 *
 *
**/