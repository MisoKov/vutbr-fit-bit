/**
 *	@file	htab_bucket_count.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	vrátí počet prvků pole (.arr_size)
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

size_t htab_bucket_count(const htab_t * t)
{
	return t->arr_size;
}