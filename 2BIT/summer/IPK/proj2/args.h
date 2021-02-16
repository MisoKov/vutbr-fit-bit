/**
 *	@file	args.h
 *  @author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Header file for args.c
 *	@note	Compiler: gcc 7.5
 */

#ifndef ARGS_H
#define ARGS_H

//      INCLUDES
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>

//      PROTOTYPES
/**
 * Prints help text to stdout
 */
void print_help();

/**
 * Copies the string to dynamically allocated memory
 * @param s string that is to be copied
 * @return pointer to copied string or NULL in case of error
 */
char* copy_string_dynamic(char* s);

/**
 * Checks if string is integer
 * @param n string containing the integer
 * @return bool, true if integer false if not
 */
bool is_valid_integer(char* n);

/**
 * Checks and processes the program parameters
 * @param argc number of arguments
 * @param argv pointer to the list of arguments
 * @param interface pointer where interface name will be stored
 * @param mode pointer to where mode will be stored
 * @param num pointer to where number of packets will be stored
 * @param port pointer to where port number will be stored
 * @return OK on succes, ERR_ARGS in case of error
 */
int process_args(int argc, char *argv[], char **interface, int *mode, int *num, char **port);

#endif //ARGS_H
