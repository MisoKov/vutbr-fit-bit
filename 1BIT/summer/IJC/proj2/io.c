/**
 *	@file	io.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	fukcia int get_word(char *s, int max, FILE *f) na citanie slov zo stdin
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 * 			dorobit hlasenie o prekraceni max dlzky slova
 */	

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> //isspace
#include <string.h> //memcpy
#include "htab.h"

int get_word(char *s, int max, FILE *f)
{
	// kontrola f pointu
	if (f == NULL) {
		fprintf(stderr, "Error: get_word: Zadany NULL pointer ako argument f\n");
		return 0;
	// kontrola max dlzky slova
	}else if (max <= 0){
		fprintf(stderr, "Error: get_word: Maximalna dlzka slova musi byt vacsia ako 0, bola zadana: %d\n",max);
		return 0;
	}
	// citanie pismen slova
	char c;
	int end_of_word = 0;
	int in_word = 0;
	int idx = 0;
	int loads = 0;
	do {
		c = fgetc(f);
		// ignorovanie whitespace pred zaciatkom slova
		if (isspace(c) == 0 && in_word == 0){
			in_word = 1;
		// detekovanie konca slova
		} else if (isspace(c) && in_word == 1){
			end_of_word = 1;
			in_word = 0;
		// detekovanie EOF
		} else if (c == EOF){
			end_of_word = 1;
			in_word = 0;
		}
		// zapis znakov do slova
		if (in_word && idx < max){
			s[idx] = c;
			idx++;
		}
		loads++;
	} while (end_of_word != 1);
	if (c == EOF){
		return EOF;
	}
	s[idx] = '\0';
	if (loads > idx){
		return loads;
	} else {
		return idx;
	}
}
