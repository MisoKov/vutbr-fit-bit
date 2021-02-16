/**
 *	@file	htab_clear.c
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vymazanie vsetkych polozie hashovacej tabulky, tabulka zostane prazdna
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

void htab_clear(htab_t * t)
{
	struct htab_item *tmp;
	struct htab_item *item;
	for (size_t i = 0; i < t->arr_size; i++){
		item = t->arr_tab[i];
		while (item != NULL){
			tmp = item->next;
			free(item->key);
			free(item);
			item = tmp;
		}
	}
	t->size = 0;
	t->arr_size = 0;
}