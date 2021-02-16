/**
 *	@file	htab.c	
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Extern deklaracie pre static inline funkcie z htab.h pre linker
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */

#include "htab.h"

extern inline bool htab_iterator_valid(htab_iterator_t it);

extern inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2);