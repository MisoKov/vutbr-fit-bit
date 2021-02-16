/**
 *	@file	bit_array.c		
 *	@author	Michal Koval, FIT
 *	@date	20.3.2019
 *	@brief	Extern definicie inline funkcii v hlavickovom subore bit_array.h
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad a)
 */		

#include <stdlib.h>
#include "bit_array.h"

#ifdef USE_INLINE
extern inline unsigned long bit_array_size(bit_array_t jmeno_pole);

extern inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, unsigned long vyraz);

extern inline unsigned long bit_array_getbit(bit_array_t jmeno_pole, unsigned long index);
#endif
