/**
 *	@file	htab_lookup_add.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Vyhlada zaznam odpovedajuci retazcu key v hashovacej tabulke
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include "htab.h"
#include "htab_structs.h"

htab_iterator_t htab_lookup_add(htab_t * t, const char *key)
{
	htab_iterator_t iterator;
	iterator.ptr = NULL;
	//kotrola pointru t 
	if (t == NULL){
		fprintf(stderr, "Error: htab_lookup_add: Zadany NULL pointer ako argumet t\n");
		return iterator;
	//kotrola pointru key
	} else if (key == NULL){
		fprintf(stderr, "Error: htab_lookup_add: Zadany NULL pointer ako argumet key\n");
		return iterator;
	}
	// vyhladanie key v tabule
	int hashcode = htab_hash_function(key);
	int idx = hashcode % t->arr_size;
	struct htab_item *item = t->arr_tab[idx];
	while (item != NULL){
		if ((strcmp(key, item->key)) == 0){
			break;
		}
		item = item->next;
	}
	// pridanie noveho zaznamu
	if (item == NULL){
		// alokacia noveho itemu
		item = malloc (sizeof(struct htab_item));
		// kotrola mallocu
		if (item == NULL){
			fprintf(stderr, "Error: htab_lookup_add: Chyba alokacie pamate\n");
			return iterator;
		}
		// inicalizacia
		int key_len = strlen(key);
		item->key = malloc(sizeof(char)*(key_len+1));
		if (item->key == NULL){
			fprintf(stderr, "Error: htab_lookup_add: Chyba alokacie pamate\n");
			return iterator;
		}
		memcpy(item->key, key, key_len+1);
		item->data = 0;
		item->next = NULL;
		//zaradenie do tabulky
		if (t->arr_tab[idx] == NULL){
			t->arr_tab[idx] = item;
		} else {
			struct htab_item *tmp = t->arr_tab[idx];
			while (tmp->next != NULL){
				tmp = tmp->next;
			}
			tmp->next = item;
		}
		t->size++;
	}
	item->data++;
	iterator.ptr = item;
	iterator.t = t;
	iterator.idx = idx;
	return iterator;
}