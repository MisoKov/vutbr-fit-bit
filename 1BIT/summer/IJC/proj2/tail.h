/**
 *	@file	tail.c		
 *	@author	Michal Koval, FIT
 *	@date	07.04.2019
 *	@brief	Zdrojovy kod programu tail v C
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU2, příklad a)
 */		

#include <stdlib.h>
#include <stdio.h>
/**
 * @def MAX_LINE_SIZE
 * @brief Max dlzka riadku
 */
#define MAX_LINE_SIZE 1023
/**
 * @def DEFAULT_PRINTED_LINES
 * @brief Implicitny pocet tlacenych riadkov
 */
#define DEFAULT_PRINTED_LINES 10

/**
 * @brief      Cita riadky zo stdin
 *
 * @param[in]      arr_len  Dlzka pola
 *
 * @return     Pole s nacitanymi riadkami
 */
char **read_stdin(long *arr_len);

/**
 * @brief      Cita riadky zo subora
 *
 * @param[in]      filename  Meno subora
 * @param[in]      arr_len   Dlzka pola
 *
 * @return     Pole s nacitanymi riadkami
 */
char **read_file(char *filename, long *arr_len);

/**
 * @brief      Dealokuje pamat pola s riadkami
 *
 * @param[in]      arr      Pole
 * @param[in]  arr_len  Dlzka pola
 */
void arr_free(char **arr, long arr_len);

/**
 * @brief      Tlaci poslenych n riadkov na stdout
 *
 * @param][in]      lines    Pole s riadkami
 * @param[in]  arr_len  Dlzka pola
 * @param[in]  n        Pocet riadkov na vytlacenie
 */
void print_result(char **lines, long arr_len, long n);

/**
 * @brief      Cita riadky zo streamu
 *
 * @param[in]      source   Stream odkial citame
 * @param[in]      arr_len  Dlzka pola
 *
 * @return     Pole s nacitanymi riadkami
 */
char **read_lines(FILE *source, long *arr_len);

/**
 * @brief      Spracuje vstupne argumet
 *
 * @param[in]  argc  Pocet argumentov
 * @param[in]      argv  Pole s argumentmi
 * @param[in,out]      n     Pocet riadkov na citanie
 *
 * @return     Nazov suboru na citanie
 */
char *process_args(int argc, char* argv[], long *n);