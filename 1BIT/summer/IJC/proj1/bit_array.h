/**
 *	@file	bit_array.h	
 *	@author	Michal Koval, FIT
 *	@date	9.3.2019
 *	@brief	Hlavickovy subor s dediniciou struktury bit_array_t a makier a inlince funkcii pracujucich so strukturou
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad a)
 */		

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "limits.h"
#include "error.h"

#ifndef BIT_ARRAY_MACROS
	/**
	 * @def BIT_ARRAY_MACROS
	 * @brief makra pre pracu s polom bitov
	 */
	#define BIT_ARRAY_MACROS	
		/**
		 * @brief datovy typ pre pole bitov
		 */
		typedef unsigned long *bit_array_t;
		/**
		 * @def UL_BITS
		 * @brief velkost unsigned longu v bitoch
		 */
		#define UL_BITS (sizeof(unsigned long)*CHAR_BIT)
		/**
		 * @def TEST_BI
		 * @brief vratenie hodnoty bitu
		 */
		#define TEST_BIT(arr,i) (arr & (1UL << i))
		/**
		 * @def SET_BIT
		 * @brief nastavenie bitu na 0/1
		 */
		#define SET_BIT(arr,i,b) ((b==1) ? (arr |= (1UL << i)) : (arr &= ~(1UL << i)))
		/**
		 * @def bit_array_create
		 * @brief vytvorenie pola bitov na stacku
		 */
		#define bit_array_create(jmeno_pole,velikost) \
			unsigned long jmeno_pole[velikost%UL_BITS > 0 ? (velikost/UL_BITS) +2 : (velikost/UL_BITS) + 1] = {velikost}; \
			static_assert(velikost > 0,"Velkost pola musi byt vacisa ako 0\n");
		/**
		 * @def bit_array_alloc
		 * @brief vytvorenie pola bitov dynamicky alokovanim pamate
		 */
		#define bit_array_alloc(jmeno_pole,velikost) \
			static_assert(velikost > 0,"Velkost pola musi byt vacisa ako 0\n"); \
			bit_array_t jmeno_pole = calloc((velikost%UL_BITS > 0 ? (velikost/UL_BITS + 2) : (velikost/UL_BITS + 1)),sizeof(unsigned long)); \
			if (jmeno_pole == NULL) { \
					error_exit("bit_array_alloc: Chyba alokacie pamate\n"); \
			} \
			jmeno_pole[0] = velikost
		/**
		 * @def bit_array_free
		 * @brief uvolnenie alokovanie pamate pre pole bitov
		 */
		#define bit_array_free(jmeno_pole) free(jmeno_pole)

		#ifndef USE_INLINE // MACROS
			/**
			 * @def bit_array_size
			 * @brief vratenie velkosti bitveho pola
			 */
			#define bit_array_size(jmeno_pole) jmeno_pole[0]
			/**
			 * @def bit_array_setbit
			 * @brief nastavenie bitu na 0/1
			 */
			#define bit_array_setbit(jmeno_pole,index,vyraz) \
				do{ \
					if ((unsigned long)index >= bit_array_size(jmeno_pole)){ \
						error_exit("bit_array_setbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole)-1); \
					} else if (vyraz != 0 && vyraz != 1){ \
						error_exit("bit_array_setbit: Vyraz %d mimo rozsah 0/1\n", vyraz); \
					} \
					(SET_BIT((jmeno_pole[(1UL+index/UL_BITS)]),index%UL_BITS,vyraz)); \
				} while (0)
			/**
			 * @def bit_array_getbit
			 * @brief vratenie hodnoty bitu
			 */
			#define bit_array_getbit(jmeno_pole,index) (\
				((unsigned long)index >= bit_array_size(jmeno_pole)) ? \
					error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole)-1),1: \
				TEST_BIT(jmeno_pole[(1UL+index/UL_BITS)],index%UL_BITS))
			
		#else // USE_INLINE 
			/**
			 * @fn	static inline unsigned long bit_array_size(bit_array_t jmeno_pole)
			 * @param	jmeno_pole pointer na pole znakov obsahujuce meno pola
			 * @return	velkost bitoveho pola
			 * @brief	vrati velkost bitoveho pola
			 */
			static inline unsigned long bit_array_size(bit_array_t jmeno_pole)
			{
				return jmeno_pole[0];
			}
			/**
			 * @fn	static inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, unsigned long vyraz)
			 * @param	jmeno_pole pointer na pole znakov obsahujuce meno pola
			 * @param	index poradnie bitu v poli
			 * @param	vyraz hodnota, na ktoru bude bit nastaveny
			 * @return	void
			 * @brief	nastavy bit na 1 alebo 0
			 */
			static inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, unsigned long vyraz)
			{
					if (index >= bit_array_size(jmeno_pole)){
						error_exit("bit_array_setbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole)-1);
					} else if (vyraz != 0 && vyraz != 1){
						error_exit("bit_array_setbit: Vyraz %d mimo rozsah 0..1\n", vyraz);
					}
					SET_BIT((jmeno_pole[(1UL+index/UL_BITS)]),index%UL_BITS,vyraz);
			}
			/**
			 * @fn	static inline unsigned long bit_array_getbit(bit_array_t jmeno_pole, unsigned long index)
			 * @param	jmeno_pole pointer na pole znakov obsahujuce meno pola
			 * @param	index poradnie bitu v bitovom poli
			 * @return	hodnota bitu
			 * @brief	vrati hodnotu bitu 0/1
			 */
			static inline unsigned long bit_array_getbit(bit_array_t jmeno_pole, unsigned long index)
			{
				if (index >= bit_array_size(jmeno_pole)){
					error_exit("bit_array_getbit: Index %lu mimo rozsah 0..%lu\n", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole)-1);
				}
				return (TEST_BIT(jmeno_pole[(1UL+index/UL_BITS)],index%UL_BITS));
			}
		#endif // pre USE_INLINE
#endif //pre BIT_ARRAY_MACROS
