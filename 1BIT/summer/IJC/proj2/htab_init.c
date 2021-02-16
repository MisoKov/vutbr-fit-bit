/**
 *	@file	htab_init.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	vytvorenie a inicializacia hashovacej tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

htab_t *htab_init(size_t n)
{
	htab_t *table = malloc (sizeof(htab_t)+sizeof(struct htab_item)*n);
	// kontrola mallocu
	if (table == NULL){
		fprintf(stderr, "Error: htab_init: Nepodarila sa alokacie pamate\n");
		return NULL;
	}
	table->size = 0;
	table->arr_size = n;
	for (size_t i = 0; i < n; i++){
		table->arr_tab[i] = NULL;
	}
	return table;
}
