/**
 *	@file	htab_begin.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vrací iterátor označující první záznam hash tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

htab_iterator_t htab_begin(const htab_t * t)
{
	htab_iterator_t itr;
	// kontrola pointru t
	if (t == NULL){
		fprintf(stderr, "Error: htab_begin: Argumentu t bol predany NULL pointer\n");
		itr.t = NULL;
		return itr;
	}
	itr.t = t;
	itr.ptr = t->arr_tab[0];
	itr.idx = 0;
	while (itr.ptr == NULL){
		if ((unsigned)itr.idx < itr.t->arr_size){
			itr.idx++;
			itr.ptr = t->arr_tab[itr.idx];
		}else{
			break;
		}
	}
	return itr;
}