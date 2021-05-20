/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
if (ptrht == NULL) return; // Ak nedostaneme valídne vstupné hodnoty - ukončujeme.
	for (unsigned int i = 0; i < HTSIZE; (*ptrht)[i++] = NULL);

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/



tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if (ptrht == NULL || key == NULL) return NULL; // Ak nedostaneme valídne vstupné hodnoty - ukončujeme.
		tHTItem* searched_index = (*ptrht)[hashCode(key)];

	while (searched_index != NULL) {	
		if (searched_index->key == key) { // Našli sme hľadanú položku, Vrátime odkaz na ňu.
			return searched_index;
		}
		searched_index = searched_index->ptrnext; // Presun na ďaľšiu položku
	}
	return NULL; // Položka nie je nájdená. Vrátime hodnotu NULL.

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

if (ptrht == NULL || key == NULL) return; // Ak nedostaneme valídne vstupné hodnoty - ukončujeme.
	tHTItem* searched_item = htSearch(ptrht,key);
		if (searched_item != NULL)	searched_item->data = data; // Ak nájdeme duplicitný kľúč, kde už máme uloženú položku, tak prepíšeme len dáta.
		else if (searched_item == NULL) { // Našli sme voľné miesto.
			tHTItem *inserting_item = malloc(sizeof(tHTItem)); // Alokujeme v pamäti veľkosť položky za pomoci operácie malloc();

			if (inserting_item != NULL) {
				inserting_item->key = key;
				inserting_item->data = data;
				inserting_item->ptrnext = (*ptrht)[hashCode(key)];
				(*ptrht)[hashCode(key)] = inserting_item;
			}
			else return;	
		}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	if (ptrht == NULL || key == NULL) return NULL; // Ak nedostaneme valídne vstupné hodnoty - ukončujeme.
		tHTItem *searched_item = htSearch(ptrht, key);
			return (searched_item == NULL) ? NULL : &searched_item->data;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if (ptrht == NULL || key == NULL) return; // Ak nedostaneme valídne vstupné hodnoty - ukončujeme.
		int h_index = hashCode(key); // Hashneme na začiatku neskôr s tým budeme pracovať - nechceme zmenu 'key'. Zatiaľ... 
		tHTItem *deleting_item = (*ptrht)[h_index]; // Priradenie na začiatok zoznamu.
		tHTItem *deleting_item_prev = (*ptrht)[h_index];
		while (deleting_item != NULL && (key != deleting_item->key)) {
			// Ak sme nenašli zhodujúcu položku pokračujeme ďalej v hľadaní.
				deleting_item_prev = deleting_item;
				deleting_item = deleting_item->ptrnext;
		}
	if (deleting_item == NULL) return; // Položka s uvedením kľúčom nič nerobí - nerobíme nič.
	if (deleting_item_prev == deleting_item) { // Našlime prvú položku. Zmažeme ju.
		(*ptrht)[h_index] = deleting_item->ptrnext;
	}
	else{
		deleting_item_prev->ptrnext = deleting_item->ptrnext; // Nie je prvou položkou. Zmažeme ju. 
	}
	// Uvolnenie položky z pamäte pomocou operácie free().
	free(deleting_item);
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	if(ptrht == NULL) return;
	tHTItem *clear_item;
	unsigned int i = 0;

	while (i < HTSIZE) {
		while ((*ptrht)[i] != NULL) {
			clear_item = (*ptrht)[i];
			(*ptrht)[i] = (*ptrht)[i]->ptrnext; // Presun na ďaľší prvok, ktorý sa bude mazať. 
			free(clear_item); // Uvoľnenie položky z pamäte pomocou operácie free().
		}
	i++; // Iterácia tabuľkou po veľkosť HTSIZE
	}
	return;
}
