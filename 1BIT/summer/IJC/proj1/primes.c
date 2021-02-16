/**
 *	@file	primes.c		
 *	@author	Michal Koval, FIT
 *	@date	14.3.2019 
 *	@brief	Vyratanie poslednych 10 prvocisiel z prvych 123 milionov cisiel
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad a)
 */			   	 

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "eratosthenes.h"


#define N 123000000

int main(void)
{
	// bit_array_alloc(p,N);
	static bit_array_create(p,N);
	// vyratanie prvocisiel pouzitim algoritmu eratestonove sito
	Eratosthenes(p);
	// zapis poslednych 10 porvocisel do pola
	unsigned long last_10[10];
	int counter = 9;
	for (int i = N-1; i > 1; i--){
		if (!(bit_array_getbit(p,i))){
			last_10[counter] = i;
			counter--;
			if (counter == -1){
				break;
			}
		}
	}
	// vypis poslednych 10
	for (int i = 0; i < 10; i++){
		printf("%ld\n",last_10[i]);
	}
	//bit_array_free(p);
	return 0;
}
