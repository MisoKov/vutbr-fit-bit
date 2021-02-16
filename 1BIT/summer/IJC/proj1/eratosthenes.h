/**
 *	@file	eratosthenes.h
 *	@author	Michal Koval, FIT
 *	@date	16.3.2019 
 *	@brief	Hlavickovy subor s definicou funkcie Eratosthenes
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad a)
 */

#include "bit_array.h"
#include <math.h>

/**
 *	@fn	void Eratosthenes(bit_array_t pole)
 *	@param	pole pointer na nulovane bitove pole
 *	@return	void
 * 	@brief	nastavi bity pola na 0 ak je poradie bitu prvocislo, v opacnom pripade na 1
 */
void Eratosthenes(bit_array_t pole);
