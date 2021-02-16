/**
 *	@file	tail.c		
 *	@author	Michal Koval, FIT
 *	@date	07.04.2019
 *	@brief	Zdrojovy kod programu tail v C
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad a)
 *		-chyba pri vypise rovnakeho poctu riadkov - text2 napr
 */		
#include "tail.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

char* process_args(int argc, char* argv[], long *n)
{
	char *subor = NULL;
	if (argc < 1 || argc > 4){
		fprintf(stderr, "Zadany nespravny pocet argumentov: %d\n", argc);
		*n = -1;
		return NULL;
	// prepinac
	} else if (argc == 3 || argc == 4){
		if (strcmp(argv[1], "-n") == 0){
			// zikanie cisla z 2. parametru
			char **endptr = NULL;
			*n = strtol(argv[2], endptr, 10);
			int n_len = floor(log10(abs(*n))) + 1;
			if (argv[2][0] == '+' || argv[2][0] == '-'){
				n_len++;
			}
			int arg_len = strlen (argv[2]);
			// osetrenie nespravneho cisla pre -n
			if (n_len != arg_len){
				fprintf(stderr, "Argument pre prepinac -n nie je cislo: %s\n", argv[2]);
				*n = -1;
				return NULL;
			} else if (*n < 1){
				fprintf(stderr, "Argument pre prepinac -n musi byt > 0, je: %s\n", argv[2]);
				*n = -1;
				return NULL;
			}
			if (argc == 4){
				subor = argv[3];
			}
		} else {
			fprintf(stderr, "Zadany nespravny prepinac: %s\n", argv[1]);
			*n = -1; 
			return NULL;
		}
	// bez prepinaca
	} else if (argc == 2){
		subor = argv[1];
	}
	return subor;
}

void arr_free(char **arr, long arr_len)
{
	for (int i = 0; i < arr_len; i++){
		free(arr[i]);
	}
	free(arr);
}
char **read_lines(FILE *source, long *arr_len)
{
	// alokacia pola
	char **tmp;
	char ** lines = calloc (1,sizeof(char*));
	if (lines == NULL){
		fprintf(stderr, "Chyba alokacie pamate\n");
		return NULL;
	}
	// citanie a ukladanie riadkov do pola
	char buffer[MAX_LINE_SIZE+1];
	int buffer_c;
	int buffer_index = 0;
	long counter = 1;
	int end = 0;
	int first_over_limit = 0;
	while (1){
		buffer_c = fgetc(source);
		// kontrola konca suboru
		if (buffer_c == EOF){
			end = 1;
		}
		// pridavanie pismen do docasnej premennej
		if (buffer_index <= MAX_LINE_SIZE && buffer_c != '\n' && buffer_c != EOF){
			buffer[buffer_index] = buffer_c;
			buffer_index++;
		} else {
			if (buffer_index > MAX_LINE_SIZE){
				// riadok dlhsi ako je dany limit
				if (first_over_limit == 0){
					fprintf(stderr, "Riadok prekrocil maximalnu dlzku, zbytok riadku je ignorovany\n");
					first_over_limit = 1;
				}
				// zahodenie zvysku riadku
				while ((buffer_c = fgetc(source)) != '\n' && buffer_c != EOF) {
					continue;
				}
				// kontrola konca suboru
				if (buffer_c == EOF){
					end = 1;
				}
			}
			// kontrola konca suboru
			if (end == 1 && buffer_index == 0){
				break;
			}
			// alokacie
			tmp = lines;
			lines = realloc(lines, counter*sizeof(char*));
			if (lines == NULL){
				arr_free(tmp, counter-1);
				fprintf(stderr, "Chyba alokacie pamate\n");
				return NULL;
			}
			lines[counter-1] = calloc(buffer_index+1, sizeof(char));
			if (lines[counter-1] == NULL){
				fprintf(stderr, "Chyba alokacie pamate\n");
				arr_free(lines, counter-1);
				return NULL;
			}
			// skopirovanie docasneho riadku do pola riadkov
			memcpy (lines[counter-1], buffer, buffer_index+1);
			counter++;
			memset(buffer, 0, buffer_index+1);
			buffer_index = 0;
		}
		// ukoncenie cyklu
		if (end == 1){
			break;
		}
	}
	*arr_len = counter -1;
	return lines;
}
char **read_stdin(long *arr_len)
{
	return read_lines(stdin, arr_len);
}

char **read_file(char *filename, long *arr_len)
{
	// otvorenie suboru
	FILE *f;
	if ((f = fopen(filename,"r")) == NULL){
		fprintf(stderr, "Subor '%s' sa nepodarilo otvorit\n", filename);
		return NULL;
	}
	char ** lines = read_lines(f, arr_len);
	// uzavretie suboru
	if (fclose(f) != 0){
		fprintf(stderr, "Subor '%s' sa nepodarilo zavriet\n", filename);
		arr_free(lines, *arr_len);
		return NULL;
	}
	return lines;
}

void print_result(char **lines, long arr_len, long n)
{ 
	if (arr_len < n){
		for (int i = 0; i < arr_len; i++){
			printf("%s\n", lines[i]);
		}
	} else {
		for (int i = arr_len - n; i < arr_len; i++){
			printf("%s\n", lines[i]);
		}
	}
}

int main (int argc, char *argv[])
{
	//osetrenie argumentov
	long n = DEFAULT_PRINTED_LINES;
	char *subor = process_args(argc, argv, &n);
	if (n == -1){
		return 1;
	}
	// rozdelenie textu do riadkov
	char ** lines;
	long arr_len;
	if (subor == NULL){
		// nezadana cesta - vypis zo stdin
		lines = read_stdin(&arr_len);
	} else {
		// zadana cesta - text zo suboru
		lines = read_file(subor, &arr_len);
	}
	// kontrola chyb
	if (lines == NULL){
		return 1;
	}
	// vypis poslednych riadkov
	print_result(lines, arr_len, n);
	// uvolnenie alokovanej pamate
	arr_free(lines, arr_len);
	return 0;
}