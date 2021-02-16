/**
 *	@file	htab_free.c	
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Destruktur, dealokuje pamat hashovacej tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

void htab_free(htab_t * t)
{
	htab_clear(t);
	free(t);
}