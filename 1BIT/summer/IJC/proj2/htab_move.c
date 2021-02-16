/**
 *	@file	htab_move.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vytvorenie a inicializacia hashovacej tabulky presunom zaznamov z inej tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"

htab_t *htab_move(size_t n, htab_t *from)
{
	// inicializacia novej tabulky
	htab_t *table = htab_init(n);
	if (table == NULL){
		fprintf(stderr, "Error: htab_move: Chyba alokacie pamate\n");
		return NULL;
	}
	// prekopirovanie zaznamov zdrojovej tabulky do novej
	htab_iterator_t last = htab_end(from);
	htab_iterator_t itr = htab_begin(from);
	htab_iterator_t tmp;
	int frekv;
	const char *key;
	while (!htab_iterator_equal(itr, last)){
		frekv = htab_iterator_get_value(itr);
		key = htab_iterator_get_key(itr);
		for (int i = 0; i < frekv; i++){
			tmp = htab_lookup_add(table, key);
			if (!htab_iterator_valid(tmp)){
				htab_free(table);
				return NULL;
			}
		}
		itr = htab_iterator_next(itr);
	}
	if (htab_iterator_valid(itr)){
		frekv = htab_iterator_get_value(itr);
		key = htab_iterator_get_key(itr);
		for (int i = 0; i < frekv; i++){
			tmp = htab_lookup_add(table, key);
			if (!htab_iterator_valid(tmp)){
				htab_free(table);
				return NULL;
			}
		}
	}
	// vyprazdnenie zdrojovej tabulky
	htab_clear(from);
	return table;
}