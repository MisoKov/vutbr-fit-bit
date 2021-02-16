/**
 *	@file	eratosthenes.c		
 *	@author	Michal Koval, FIT
 *	@date	9.3.2019
 *	@brief	Implementacia algoritmu na najdenie prirodzenych cisel
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad a)
 */				

#include "eratosthenes.h"
#include <stdio.h>
#include <stdlib.h>

void Eratosthenes(bit_array_t pole)
{
	// 0 a 1 nie su prvocisla, nastavime bit na 1
	bit_array_setbit(pole,0UL,1UL);
	bit_array_setbit(pole,1UL,1UL);
	// ziskanie poctu cisel
	unsigned long N = bit_array_size(pole);
	// eratesthonove sito
	for (unsigned long i = 2; i < sqrt(N); i++){
		if (!(bit_array_getbit(pole,i))){
			for (unsigned long j = i; j*i < N ; j++){
				if (!(bit_array_getbit(pole,(unsigned long)j*i))){
					bit_array_setbit(pole,(unsigned long)i*j,1LU);
				}
			}
		}
	}
}
