/**
 *	@file	htab_end.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vrací iterátor označující (neexistující) první záznam za koncem hash tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */

#include "htab.h"
#include "htab_structs.h"

htab_iterator_t htab_end(const htab_t * t)
{
	htab_iterator_t itr;
	// kontrola pointru t
	if (t == NULL){
		fprintf(stderr, "Error: htab_end: Argumentu t bol predany NULL pointer\n");
		itr.t = NULL;
		return itr;
	}
	itr.t = t;
	// najdenie indexu pola, ideme odzadu
	int idx = t->arr_size-1;
	while (t->arr_tab[idx] == NULL && idx > 0){
		idx--;
	}
	itr.idx = idx;
	itr.ptr = t->arr_tab[idx];
	// prva polozka je zaroven aj posledna
	if (t->arr_tab[idx] == NULL){
		return itr;
	}
	// nadenie polozky
	while (itr.ptr->next != NULL){
		itr.ptr = itr.ptr->next;
	}
	return itr;
}