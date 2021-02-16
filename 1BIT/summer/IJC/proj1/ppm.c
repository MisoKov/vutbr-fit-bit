/**
 *	@file	ppm.c		
 *	@author	Michal Koval, FIT
 *	@date	18.3.2019
 *	@brief	Implementacia funkcii pracujucich s ppm P6 formatom 
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad b)
 */			   		

#include "ppm.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void close_file(FILE * subor,const char * filename)
{
	if (fclose(subor) == EOF){
		warning_msg("Subor %s sa nepodarilo zavriet\n",filename);
	}
}
struct ppm* ppm_read(const char * filename)
{
	// otvorenie suboru
	 FILE * subor = fopen(filename,"rb");
	 if (subor == NULL){
		error_exit("Subor '%s' sa nepodarilo otvorit\n",filename);
		return NULL;
	}
	// kontrola magic number
	char magic [2];
	if (fscanf(subor,"%s\n",magic) != 1){
		warning_msg("Nepodarilo sa nacitat format obrazku '%s'\n",filename);
		close_file(subor,filename);
		return NULL;
	}
	if (strcmp(magic,MAGIC_NUM) != 0){
		warning_msg("Format obrazku '%s' ma byt P6, je: %s\n",filename,magic);
		close_file(subor,filename);
		return NULL;
	}
	// nacitanie rozmerov
	unsigned width;
	unsigned heigth;
	if (fscanf(subor,"%u %u\n",&width, &heigth) != 2){
		warning_msg("Z obrazku '%s' sa nepodarilo nacitat rozmery\n",filename);
		close_file(subor,filename);
		return NULL;
	}
	if (width <= 0 || heigth <= 0){
		warning_msg("Rozmery obrazku '%s' musia byt vacsie ako 0, width: %d heigth: %d\n",filename,width,heigth);
		close_file(subor,filename);
		return NULL;
	} else if (width >8000 || heigth > 8000){
		warning_msg("Rozmery obrazku '%s' nemozu byt vacsie ako 8000, width: %d heigth: %d\n",filename,width,heigth);
		close_file(subor,filename);
		return NULL;
	}
	// kontrola farebnej hlbky
	unsigned max_val;
	if (fscanf(subor,"%u\n",&max_val) != 1){
		warning_msg("Z obrazku '%s' sa nepodarilo nacitat maximalnu farebnu hlbku\n",filename);
		close_file(subor,filename);
		return NULL;
	}
	if (max_val != MAX_VAL){
		warning_msg("Maximalna farebna hlba obrazku '%s' ma byt %d, je: %d\n",filename,MAX_VAL,max_val);
		close_file(subor,filename);
		return NULL;
	}
	// alokacia pamate pre obrazok
	struct ppm *obrazok = malloc (sizeof(struct ppm)+3*width*heigth);
	if (obrazok == NULL){
		warning_msg("Zlyhanie alokacie pamate pre obrazok\n");
		close_file(subor,filename);
		return NULL;
	}
	// nastavenie hodnot struktury ppm
	obrazok->xsize = width;
	obrazok->ysize = heigth;
	int byte;
	for (unsigned i = 0; i < 3*width*heigth; i++){
		if ((byte = getc(subor)) == EOF){
			warning_msg("V obrazoku %s nebol najdeny dostatok dat pre jeho rozlisenie: %dx%d\n",filename,width,heigth);
			close_file(subor,filename);
			ppm_free(obrazok);
			return NULL;
		}
		obrazok->data[i] = byte;
	}
	close_file(subor,filename);
	return obrazok;
}

void ppm_free(struct ppm *p)
{
	free(p);
}
