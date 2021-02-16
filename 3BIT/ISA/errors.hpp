/**
 *	@file	errors.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	This headers file contains defines of errors and their meanings
 *	@note	Compiler: gcc 7.5
 */

#ifndef ISA_ERRORS_H
#define ISA_ERRORS_H

/**
 * Operation failed
 */
#define FAIL -1

/**
 * Success, no problem
 */
#define OK 0

/**
 * Error has occurred due to program argument processing
 */
#define ERR_ARGS 1

/**
 * Errors that occurred during establishment of connection
 */
#define ERR_CONN 2

/**
 * Errors caused by unsuccessful communication
 */
#define ERR_SSL 3

/**
 * Errors caused by bad HTTP requests handling
 */
#define ERR_HTTP 4

#endif //ISA_ERRORS_H
