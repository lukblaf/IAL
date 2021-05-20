
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
    /*
    ** Narazili sme na pravú zátvorku, takže vyprázňujeme zásobník do výstupného reťazca až dokým nenarazíme na pravú zátvorku.
    ** POZOR! Pravá zátvorka nejde do výstupného reťazca, takže tej sa zbavíme, ak na ňu narazíme počas vyprázdňovania zásobníka.
    ** Očakávaný výstup: Spracovaný výraz od pravej po ľavú zátvorku do výstupného reťazca bez ')' a '(' zátvorky.
    */
    while ( !stackEmpty(s) ) {
        char tmp; stackTop(s, &tmp);
        if (tmp == '(') {
            stackPop(s);
            return;
        }
        postExpr[*postLen] = tmp;
        *postLen = *postLen + 1; 
        stackPop(s);
    }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
    // Ak je prvok operátor a zásobník je prázdny, tak ho vložíme do zásobníka.
    if ( stackEmpty(s) ) {
        stackPush(s, c);
        return;
    }

    char temp; stackTop(s,&temp); // Zoberieme si prvok z vrchu zásbníka do pomocnej premennej kvôli testom výskytu.
    // Ak je prvok ľavá zátvorka, tak ho vložíme do zásobníka.
    if (temp == '(') {
        stackPush(s, c);
    }
    // Ak je na vrchu zásobníku operátor s nižšou prioritou, tak vkladáme do zásbíka operátor s prioritou vyššou.
    
    else if ((temp == '+' || temp == '-') && (c == '*' || c == '/')) { 
        stackPush(s, c);
    }
    // Ak je na vrchu zásobníku operátor s vyššou prioritou vykonáme: 
    // 1. Vložíme do výstupného reťazca. 2. Odstránime zo zásobníka. 3. opakujeme doOperation(), dokým sa nám prvok nepodarí dať na vrchol zásobníka. 
    
    else if ((temp == '*' || temp == '/') && (c == '+' || c == '-')) {
        postExpr[*postLen] = temp;
        *postLen = *postLen + 1; 
        stackPop(s);
        doOperation(s, c, postExpr, postLen);  
    }
    // Ak je na vrchu zásobníku operátor s rovnakou prioritou vykonáme: 
    // 1. Vložíme do výstupného reťazca. 2. Odstránime zo zásobníka. 3. opakujeme doOperation(), dokým sa nám prvok nepodarí dať na vrchol zásobníka.
    
    else if ((temp == '+' || temp == '-') && (c == '+' || c == '-')) {
        postExpr[*postLen] = temp;
        *postLen = *postLen + 1; 
        stackPop(s);
        doOperation(s, c, postExpr, postLen);  
    }
    
    else if ((temp == '*' || temp == '/') && (c == '*' || c == '/')) {
        postExpr[*postLen] = temp;
        *postLen = *postLen + 1; 
        stackPop(s);
        doOperation(s, c, postExpr, postLen);  
    }
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char *infix2postfix (const char* infExpr) {
    char *postExpr = (char *) malloc(MAX_LEN * sizeof(char)); // Alokácia miesta v pamäti pre výstupný reťazec
    if (!postExpr){  // Kontrola operácie malloc().
        return NULL;
    }

    tStack *s = (tStack *) malloc(sizeof(tStack));
    if (!s) { // Kontrola operácie malloc().
        free(postExpr); // Uvoľníme alokovanú pamäť pre výstup
        return NULL;
    }

    stackInit(s); // Inicializácia zásobníka 

    char c = *infExpr; // deklarovanie ukazovateľa na špecifický a prehľadávaný znak v reťazci 
    unsigned int postLen = 0; // Inicializácia výstupného reťazca resp. inicializácia na prvé miesto tam, kde sa bude vkladať.

    while (c != '\0') {  
        if ((c >= '0' && c<='9') || (c >='a' && c<='z') || (c >='A' && c<= 'Z')){ // Ak spracúvame operand pošleme ho rovno do výstupného reťazca.
            postExpr[postLen++] = c;  
        }
        else if (c == '(') { // Ak spracúvame ľavú zátvorku vložíme ju do zásobníka.
            stackPush(s,c);
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' ) { // Ak spracúvame operátor, tak to posúvame pomocnej funkcii doOperation().
            doOperation(s, c, postExpr, &postLen);
        }
        else if (c == ')'){ // Ak spracúvame pravú zátvorku, tak to posúvame pomocnej funkcii untilLeftPar().
            untilLeftPar(s, postExpr, &postLen);            
        }
        else if (c == '='){ // Ak sme narazili na rovnítko, tak vyprázňujeme obsah zásoníka do výstupného reťazca.
            while (!stackEmpty(s)){
                char temp; stackTop(s, &temp);
                stackPop(s); 
                postExpr[postLen++] = temp;
            }
            postExpr[postLen++] = '='; // Pridanie rovnítka ako omedzovača výrazu na koniec spracúvaného reťazca. (while nám to rovníkto odsekol. :( )
        }
        c = *(++infExpr);
    }

    postExpr[postLen++] = '\0';  // ukončenie reťazca ukončovacím znakom '\0'. (pozn. pri cykle while sme si ho odsekli)
    free(s); // Dealokácia miesta v pamäti zásobníka
    
    return postExpr; // Výstup funkcie navráti na výstup postfixový výraz
}
/* Konec c204.c */
