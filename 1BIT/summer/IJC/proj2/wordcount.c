/**
 *	@file	wordcount.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Pocita frekvenciu slov zo stdin
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */
#include <stdlib.h>
#include "htab.h"

int main (void){
	// inicializacia hash tabulky
	htab_t *htab = htab_init(HTAB_SIZE);
	if (htab == NULL){
		return 1;
	}
	// inicializacia premmenych
	int len;
	htab_iterator_t itr;
	int warning = 0;
	char *word = malloc (sizeof(char)*(WORD_MAX_LEN+1));
	if (word == NULL){
		fprintf(stderr, "Error: wordcount: Chyba alokacie pamate\n");
		htab_free(htab);
		return 1;
	}
	// citanie slov
	while ((len = get_word(word, WORD_MAX_LEN, stdin)) != EOF){
		if (len == 0){
			free(word);
			htab_free(htab);
			return 1;
		} else if (len > WORD_MAX_LEN && warning == 0){
			fprintf(stderr, "Log: get_word: Prekrocena maximalna dlzka slova, zbytok slova je ignorovany\n");
			warning = 1;
		}
		itr = htab_lookup_add(htab, word);
		if (itr.ptr == NULL){
			htab_free(htab);
			free(word);
			return 1;
		}
	}
	// uvolnenie mallocu pre word
	free(word);
	// vypis frekevencie
	htab_iterator_t last = htab_end(htab);
	itr = htab_begin(htab);
	while (!htab_iterator_equal(itr, last)){
		printf("%s\t%d\n",htab_iterator_get_key(itr),htab_iterator_get_value(itr));
		itr = htab_iterator_next(itr);
	}
	if (htab_iterator_valid(itr)){
		printf("%s\t%d\n",htab_iterator_get_key(itr),htab_iterator_get_value(itr));
	}

	// vypis tabulky vytvorenej s move - pre kontrolu spravnosti mala
	// by byt rovnaka plus doplnena o 20 vyskutov slova "teestiiing"
	#ifdef TEST
		// vytvorenie tabulky
		htab_t *moved_table = htab_move(HTAB_SIZE+312323, htab);
		// pridanie 20 krat slovo "teestiiing" uz k povodnym zaznamom
		htab_iterator_t tmp;
		for (int i = 0; i < 20; i++){
			tmp = htab_lookup_add(moved_table, "teestiiing");
			if (!htab_iterator_valid(tmp)){
				htab_free(moved_table);
				return 0;
			}
		}
		// vypis tabulky
		printf("-----------------------------\n");
		last = htab_end(moved_table);
		itr = htab_begin(moved_table);
		while (!htab_iterator_equal(itr, last)){
			printf("%s\t%d\n",htab_iterator_get_key(itr),htab_iterator_get_value(itr));
			itr = htab_iterator_next(itr);
		}
		if (htab_iterator_valid(itr)){
			printf("%s\t%d\n",htab_iterator_get_key(itr),htab_iterator_get_value(itr));
		}
		// dealokacia presunutej hash tabulky
		htab_free(moved_table);
	#endif // TEST
	
	// dealokacia hashovacej tabulky
	htab_free(htab);
	return 0;
}