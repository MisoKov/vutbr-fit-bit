/**
 *	@file	htab_iterator_set_value.c
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Nastavi hodnotu argument it na argument val
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

int htab_iterator_set_value(htab_iterator_t it, int val)
{
	if (!htab_iterator_valid(it)) {
		fprintf(stderr,"Error: htab_iterator_set_value: Ciel argumentu it neexistuje\n");
		return -1;
	} else {
		it.ptr->data = val;
		return val;
	}
}