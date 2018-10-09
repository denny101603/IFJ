# IFJ

*Javadoc*
Obecně psát komentáře česky bez háčků a bez čárek. 

•	Do každého souboru dávat tuto preambuli:

/***************************************
* 	IFJ projekt 2018                   *
* 						                         *
*	Autori:					                     *
*	Jan Beran (xberan43)			           *
*	Daniel Bubenicek (xbuben43)		       *
*	Jan Carba (xcarba00)			           *
*	Matej Jelinek (xjelen49)			       *
*						                           *
*	Tento soubor je soucasti IFJ projektu.*
*	Kopirovani prinese zhoubu nam i vam, 	*
*	tak to prosim nedelejte. Diky! :D 	  *
***************************************/

•	Do každého souboru dále tuto hlavičku: 

/**
*	@file nazev_souboru.[c,h]
*	@author Vase Jmeno 
*	@brief Kratky popis v cestine bez diakritiky
*
*	Pripadny podrobnejsi popis
*
*/

•	Nové datové typy (typedef) se komentují takto:

/**
*	@brief Kratky popis
*
*/

•	Funkce se komentují takto: 

/**
*	@brief Kratky popis
*	@author jmeno_autora
*	@param jmeno_parametru – hodnoty
*	@param[in, out] jmeno_ukazatele – [in, out] = vstupni, vystupni. Muze byt i jen [in] nebo [out]
*	@return navratova_hodnota, pokud existuje
*	@note poznamka (nepovinne)
*	@warning varovani – SEM PSAT, POKUD SE JEDNA O PRIVATNI FUNKCI
*	
*	Pripadny delsi popis
*
*/
