/**
 *	@file	errors.h
 *  @author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Definitions of error codes
 */

#ifndef ERRORS_H
#define ERRORS_H

#define OK 0 // success, no error occurred
#define ERR_ARGS 1 // invalid arguments, missing options
#define ERR_INTERFACE 2 // when no interface was found, when incompatible interface was entered
#define ERR_FILTER 3 // when filter expression couldn't be compiled or when it couldn't be set for device
#define ERR_IP_H 4 // when size of IP header is less than MIN_IP_HEADER_SIZE
#define ERR_TCP_H 5 // when size of TCP header is less than MIN_TCP_HEADER_SIZE
#define ERR_PROTOCOL 6 // when other than TCP or UDP protocol was detected
#define ERR_ADDR_CON 7 // when IP address couldn't be converted from network byte code to string
#define END_HELP 8 // program finished successfully by printing help

#endif //PROJ2_ERRORS_H
