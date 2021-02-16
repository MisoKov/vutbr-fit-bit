/**
 *	@file	error.c
 *	@author	Michal Koval, FIT
 *	@date	14.3.2019 
 *	@brief	Implementacia funkcii na chybove hlasenia
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad b)
 */		   	 

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "error.h"

void warning_msg(const char *fmt, ...)
{
	fprintf(stderr, "CHYBA: ");
	va_list arguments;
	va_start(arguments, fmt);
	vfprintf(stderr, fmt, arguments);
	va_end(arguments);
}

void error_exit(const char *fmt, ...)
{
	fprintf(stderr, "CHYBA: ");
	va_list arguments;
	va_start(arguments, fmt);
	vfprintf(stderr, fmt, arguments);
	va_end(arguments);
	exit(1);
}
