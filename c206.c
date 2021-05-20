 
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

/*
** Zoznam je prazdny takže ukazatele su nastavene na prazdnu hodnotu, čiže NULL.
*/
    L->Act = NULL;
    L->Last = NULL;
    L->First = NULL;
	
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    if (L->First == NULL) return;   // Ak máme prvý prvok zoznamu prázdny, tak predpokladáme, že nemáme, čo rušiť.

    struct tDLElem *element = L->First; // Priradenie referencie na prvý prvok zoznamu.

    while (element != NULL) { // Iterujeme zoznamom až po jeho koniec.
        struct tDLElem *next = element->rptr; // Priradenie referencie na nasledujúci susedný prvok.
        free(element); // Uvoľnenie na aktuálne ukazujúci prvok ukazateľom *element operáciou free().
        element = next; // Prejdeme na ďaľší prvok v poradí.
    }

    // Nadstavenie ukazateľov na prvý, posledný a aktuálny prvok na hodnotu NULL.
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;  
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    // vyhradenie miesta pre nový prvok
    struct tDLElem *newElemPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));

    //kontrola operácie malloc();
    if (newElemPtr == NULL){
        DLError();
    }

    newElemPtr->data = val;
    newElemPtr->rptr = L->First; // nový ukazateľ pravého prvku ma referenciu na ukazateľ prvého prvku
    newElemPtr->lptr = NULL; //nový ukazateľ ľavého prvku ma refrenciu na hodnotu NULL

    // zoznam už mal prvý prvok, takže by nemal byť hodnoty NULL
    if (L->First != NULL){
        L->First->lptr = newElemPtr; // prvý prvok bude ukazovať na nový prvok doľava
    }
    else{
        L->Last = newElemPtr; //vloženie do prázdneho zoznamu 
    }
    L->First = newElemPtr; //korekcia ukazovateľa začiatku
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	//vyhradenie miesta pre nový prvok
    struct tDLElem *newElemPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));

    //kontrola operácie malloc();
    if (newElemPtr == NULL){
        DLError();
    }
	
    newElemPtr->data = val;
    newElemPtr->lptr = L->Last; // nový ukazateľ ľavého prvku ma referenciu na ukazateľ posledného prvku
    newElemPtr->rptr = NULL; //nový ukazateľ pravého prvku ma refrenciu na hodnotu NULL

    // zoznam už mal posledný prvok, takže by nemal byť hodnoty NULL
    if (L->Last != NULL){
        L->Last->rptr = newElemPtr; // posledný prvok bude ukazovať na nový prvok doľava
    }
    else{
       L->First = newElemPtr; //vloženie do prázdneho zoznamu 
    }
    L->Last = newElemPtr; //korekcia ukazovateľa začiatku
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First; //prvému priradím privilégium aktívneho
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last; //poslednému nastaví privilégium aktívneho
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if ((L->First != NULL) || (L->Last != NULL)){
        *val = L->First->data; //Ak je zoznam aktívny, tak vraciame prostredníctvom parametru val hodnotu prvého prvku zoznamu L.
        return;
    } 
        DLError();
        return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if ((L->First != NULL) || (L->Last != NULL)){
        *val = L->Last->data; //Ak je zoznam aktívny, tak vraciame prostredníctvom parametru val hodnotu posledného prvku zoznamu L.
        return;
    } 
        DLError();
        return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    struct tDLElem *elemPtr;
    if (L->First != NULL) {
        elemPtr = L->First;
        if (L->Act == L->First){ // Prvému prvku skontrolujeme aktivitu a následne mu ju zrušíme.
            L->Act = NULL;  // Rušenie aktivity.
        }
        if (L->First == L->Last){ //Ak mal zoznam jediný prvok (čo znamená že je daný prvok prvý aj posledný), tak ho zrušíme
            L->First = NULL;
            L->Last = NULL;    
        }
        else{
            L->First = L->First->rptr; //Aktualizácia začiatku zoznamu.
            L->First->lptr = NULL; // Nasledujúci prvok zoznamu nastavíme na hodnotu NULL.
        }
        free(elemPtr); // Uvoľnenie pamäte za pomoci operácie free().
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
    struct tDLElem *elemPtr;
    if (L->Last != NULL){
        elemPtr = L->Last;
        if (L->Act == L->Last){ // Poslednému prvku skontrolujeme aktivitu a následne mu ju zrušíme.
            L->Act = NULL;  // Rušenie aktivity.
        }
        if (L->Last == L->First){ //Ak mal zoznam jediný prvok (čo znamená že je daný prvok prvý aj posledný), tak ho zrušíme
            L->Last = NULL;
            L->First = NULL;    
        }
        else{
            L->Last = L->Last->lptr; //Aktualizácia konca zoznamu.
            L->Last->rptr = NULL; // Prechadzajúci prvok zoznamu nastavíme na hodnotu NULL.
        }
        free(elemPtr); // Uvoľnenie pamäte za pomoci operácie free().
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

/*
** Kontrola, či máme aktívny prvok.
*/
    if (L->Act != NULL){
        if (L->Act->rptr != NULL){ // Kontrola, či máme vlastne nejaký prvok na rušenie
            struct tDLElem *elemPtr;
            elemPtr = L->Act->rptr; // Ukazovateľ na rušený prvok.
            L->Act->rptr = elemPtr->rptr; // Priradenie rušeného prvku.
            if (elemPtr == L->Last){ // Ak bude rušený posledný prvok,
                L->Last = L->Act;   // tak posledný prvok bude aktívnym prvkom.
            }
            else{ // prvok za zrušením prvkom ukazuje doľava na aktuálny prvok
               elemPtr->rptr->lptr = L->Act;
            }
            free(elemPtr);
        } // Vykoná sa, ak máme nejaký prvok na rušenie. 
    } // Vykoná sa, ak máme aktívny prvok.
		
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

/*
** Kontrola, či máme aktívny prvok.
*/
    if (L->Act != NULL){
        if (L->Act->lptr != NULL){ // Kontrola, či máme vlastne nejaký prvok na rušenie
            struct tDLElem *elemPtr;
            elemPtr = L->Act->lptr; // Ukazovateľ na rušený prvok.
            L->Act->lptr = elemPtr->lptr; // Priradenie rušeného prvku.
            if (elemPtr == L->First){ // Ak bude rušený prvý prvok,
                L->First = L->Act;   // tak prvý prvok bude aktívnym prvkom.
            }
            else{ // prvok pred zrušením prvkom ukazuje doprava na aktuálny prvok
               elemPtr->lptr->rptr = L->Act;
            }
            free(elemPtr);
        } // Vykoná sa, ak máme nejaký prvok na rušenie. 
    } // Vykoná sa, ak máme aktívny prvok.
		
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL){
        struct tDLElem *newElemPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));
            if (newElemPtr == NULL){ //kontrola operácie malloc();
            DLError();
            }
        newElemPtr->data = val;
        newElemPtr->rptr = L->Act->rptr;
        newElemPtr->lptr = L->Act;
        L->Act->rptr = newElemPtr; // Priradenie ľavého suseda na nový prvok
        // Vkladanie za posledný prvok.
        if (L->Act == L->Last){ 
            L->Last = newElemPtr;
        }
        else{ //Priradenie pravého suseda na nový prvok.
            newElemPtr->rptr->lptr = newElemPtr;
        }
    } // Vykoná sa, ak máme aktívny prvok.
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL) {
        struct tDLElem *newElemPtr = (struct tDLElem *) malloc(sizeof(struct tDLElem));
            
            if (newElemPtr == NULL){ //kontrola operácie malloc();
            DLError();
            }
        newElemPtr->data = val;
        newElemPtr->lptr = L->Act->lptr;
        newElemPtr->rptr = L->Act;
        L->Act->lptr = newElemPtr; // Priradenie ľavého suseda na nový prvok
        // Vkladanie pred prvý prvok.
        if (L->Act == L->First){ 
            L->First = newElemPtr;
        }
        else{ //Priradenie ľavého suseda na nový prvok.
            newElemPtr->lptr->rptr = newElemPtr;
        }
    } // Vykoná sa, ak máme aktívny prvok.
	
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L->Act != NULL){
        *val = L->Act->data; //Ak je zoznam aktívny, tak vraciame prostredníctvom parametru val hodnotu aktívneho prvku zoznamu L.
        return;
    }
        DLError();  // Ak je zoznam neaktívny vypíšeme chybu. 
        return;

}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if (L->Act){
        L->Act->data = val; //Prepísanie obsahu aktuálneho prvku za pomoci vstupného parametru funkcie 'val'.
    }
}


void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

/*
** Keďže chceme posunúť aktivitu na následujúci prvok zoznamu, je potrebné 
** skontrolovať, či aktívny prvok nie je posledným. Pretože "ničomu" aktivitu
** nechceme pridávať(pozn. stále to bude nič :)).
** 1) Ak je posledným prvkom, tak aktívny prvok stráca toto privilégium. Priradíme mu hodnotu NULL.
** 2) Ak nie je posledným prvkom, tak nasledujúcemu prvku priradíme aktivitu. 
*/
    if (L->Act != NULL) {
        if (L->Last != NULL) {
            L->Act = L->Act->rptr;
        }
        else {
            L->Act = NULL;
        }
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

/*
** Keďže chceme posunúť aktivitu na predchádzajúci prvok zoznamu, je potrebné 
** skontrolovať, či aktívny prvok nie je prvým. Pretože "ničomu" aktivitu
** nechceme pridávať(pozn. stále to bude nič :)).
** 1) Ak je prvým prvkom, tak aktívny prvok stráca toto privilégium. Priradíme mu hodnotu NULL.
** 2) Ak nie je prvým prvkom, tak predchádzajúcemu prvku priradíme aktivitu. 
*/
    if (L->Act != NULL) {
        if (L->First != NULL) {
            L->Act = L->Act->lptr;
        }
        else{
            L->Act = NULL;
        }    
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	return L->Act != NULL; // Ak je zoznam aktívny, tak funkcia navracia hodnotu TRUE, ak nie je, tak hodnotu FALSE.
}

/* Konec c206.c*/
