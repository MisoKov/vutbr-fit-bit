/**
 *	@file	htab_size.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	vrátí počet prvků tabulky (.size)
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

size_t htab_size(const htab_t * t)
{
	return t->size;
}