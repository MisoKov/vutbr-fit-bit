/**
 *	@file	htab_iterator_get_value.c	
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vracia hodnotu argumentu it
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

int htab_iterator_get_value(htab_iterator_t it)
{
	if (!htab_iterator_valid(it)) {
		fprintf(stderr,"Error: htab_iterator_get_value: Ciel argumentu it neexistuje\n");
		return -1;
	} else {
		return it.ptr->data;
	}
}