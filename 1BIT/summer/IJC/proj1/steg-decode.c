/**
 *	@file	steg-decode.c
 *	@author	Michal Koval, FIT
 *	@date	18.3.2019
 *	@brief	Ziskanie tajnej spravy z obrazku pomocou stenografie
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad b)
 */ 				   	

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "ppm.h"
#include "error.h"
#include "eratosthenes.h"
#include "bit_array.h"

int main (int argc, char *argv[])
{
	// kontrola argumentu
	if (argc != 2){
		error_exit("Program ma mat iba 1 argument, boli zadane: %d\n",argc-1);
	}
	// nacitanie obrazku do struktury
	struct ppm* obrazok = ppm_read(argv[1]);
	if (obrazok == NULL){
		return 1;
	}
	// vytvorenie pola s prvocislami
	bit_array_create(primes, 8000*8000*3);
	Eratosthenes(primes);
	// najdenie tajnej spravy
	int c = 0;
	int counter = 0;
	for(unsigned long i = 19; i < bit_array_size(primes); i++)
	{
		if(bit_array_getbit(primes,i) == 0UL){
			SET_BIT(c,counter,TEST_BIT(obrazok->data[i],0));
			counter++;
		}
		// vsetkych 8 bitov naplnenych - tlac znaku
		if (counter == 8){
			// koniec spravy
			if(c == '\0'){
				putc('\n',stdout);
				ppm_free(obrazok);
				return 0;
			}
			printf("%c",c);
			counter = 0;
			c = 0;
		}
	}
	ppm_free(obrazok);
	warning_msg("Sprava nebola ukoncena znakom \\0!");
	return 1;
}
