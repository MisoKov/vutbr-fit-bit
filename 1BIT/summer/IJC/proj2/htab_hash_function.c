/**
 *	@file	htab_hash_function.c		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	hashovacia funckia
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include <stdint.h>

#ifdef HASHTEST
	// K&R prva edicia, len tak zo srandy pre vyskysanie ake to bude pomale
	unsigned int htab_hash_function(const char *str) {
		unsigned int hash = 0;
		int c;
		while (c = *str++)
		    hash += c;
		return hash;
	}
#else
	unsigned int htab_hash_function(const char *str) {
		uint32_t h=0; // musí mít 32 bitů
	 	const unsigned char *p;
	 	for(p=(const unsigned char*)str; *p!='\0'; p++){
	 		h = 65599*h + *p;
	 	}
	 	return h;
	}
#endif // HASHTEST