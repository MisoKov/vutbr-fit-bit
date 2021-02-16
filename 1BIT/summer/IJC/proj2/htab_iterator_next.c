/**
 *	@file	htab_iterator_next.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Posun iterátoru na další záznam v tabulce (nebo na htab_end(t))
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
	htab_iterator_t itr;
	itr.t = it.t;
	itr.idx = it.idx;
	if (it.ptr->next == NULL) {
		while (it.t->arr_size > (unsigned)itr.idx){
			itr.idx++;
			itr.ptr = it.t->arr_tab[itr.idx];
			if (itr.ptr != NULL){
				break;
			}
		}
	} else {
		itr.ptr = it.ptr->next;
	}
	return itr;
}