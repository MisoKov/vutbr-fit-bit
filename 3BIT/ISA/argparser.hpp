/**
 *	@file	argparser.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Header file for arguments parsing part of program
 *	@note	Compiler: gcc 7.5
 */
#ifndef ISA_ARGPARSER_H
#define ISA_ARGPARSER_H

#include "errors.hpp"
#include <string.h>
#include <iostream>
#include <string>

/**
 * Structure used to return processed arguments
 */
typedef struct {
    std::string token;
    bool help;
    bool verbose;
    int ret_code;
} Args;

/**
 * Handles and processes the program arguments
 * @param argc number of arfuments
 * @param argv list of arguments
 * @return Args structure containing processed arguments
 */
Args handle_args(int argc, char *argv[]);

/**
 * Prints help message to stdout
 */
void print_help();

#endif //ISA_ARGPARSER_H
