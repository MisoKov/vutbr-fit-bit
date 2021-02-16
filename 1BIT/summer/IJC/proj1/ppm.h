/**
 *	@file	ppm.h
 *	@author	Michal Koval, FIT
 *	@date	18.3.2019  
 *	@brief	Protoypy funkcii v ppm.c
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad b)
 */		   	 
 
#define MAGIC_NUM "P6"
#define MAX_VAL 255
#include <stdio.h>

/**
 *	@brief	Struktura obsahujuca data z ppm obrazku
 *	@struct	ppm
 *	@var	ppm::xsize	sirka
 *	@var	ppm::ysize	vyska
 *	@var	ppm::data	byty reprezentujuce RGB
 *	
 */
struct ppm {
	unsigned xsize; 
	unsigned ysize; 
	char data[]; // RGB bajty, celkem 3*xsize*ysize 
};

/**
 *	@fn	struct ppm * ppm_read(const char * filename)
 *	@param	filename nazov ppm obrazku
 *	@return ppm* pointer na alokovanu strukturu s datmy
 *	@brief	nacitata data z obrazku a ulozi do alokovanej struktury
 */
struct ppm* ppm_read(const char * filename);

/**
 *	@fn	void ppm_free(struct ppm *p)
 *	@param	p pointer na struct ppm
 * 	@return	void
 *	@brief	dealokuje pamat na ktoru ukazuje pointer p
 */
void ppm_free(struct ppm *p);

/**
 *	@fn	void close_file(FILE * subor,const char * filename)
 *	@param	subor pointer na subor
 *  @param	filename nazov otvoreneho suboru
 * 	@return	void
 *	@brief	uzatvori subor, v pripade chyby vypise chybove hlasenie
 */
void close_file(FILE * subor,const char * filename);
