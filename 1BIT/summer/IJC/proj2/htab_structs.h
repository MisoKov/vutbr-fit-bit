/**
 *	@file	htab_structs.h		
 *	@author	Michal Koval, FIT
 *	@date	14.04.2019
 *	@brief	Definicie struktur hashovacej tabulky
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad b)
 */	

#include <stdlib.h>

/**
 * @brief      Struktura shahovacej tabulky
 */
struct htab {
	size_t size; /** <Pocet zaznamov hashovacej tabulky> */
	size_t arr_size; /** <Velkost pola ukazatelov arr_tab> */
	struct htab_item *arr_tab[]; /** <Pole zaznamov/bucketov> */
};

/**
 * @brief      Struktura zaznamu hashovacej tabuly 
 */
struct htab_item {
	char *key; /** <Slovo> */
	int data; /** <Frekvencia slova> */
	struct htab_item *next; /** <Pointer na dalsi zaznam v poli> */
}; 