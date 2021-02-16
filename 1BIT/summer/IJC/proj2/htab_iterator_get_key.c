/**
 *	@file	htab_iterator_get_key.c	
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vracia kluc arguemntu it
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

const char * htab_iterator_get_key(htab_iterator_t it)
{
	if (!htab_iterator_valid(it)) {
		fprintf(stderr,"Error: htab_iterator_get_key: Ciel argumentu it neexistuje\n");
		return NULL;
	} else {
		return it.ptr->key;
	}
}