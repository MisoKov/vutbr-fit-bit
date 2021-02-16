/**
 *	@file	main.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Header file of main program implementation of Discord bot
 *	@note	Compiler: gcc 7.5
 */

#ifndef ISA_MAIN_H
#define ISA_MAIN_H

#include "network.hpp"
#include "argparser.hpp"
#include "errors.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <time.h>
#include <utility>
#include <sstream>
#include <csignal>
#include <chrono>
#include <ctime>
#include <queue>

/**
 * SSL_CTX context structure for ssl, only one should be used during the program lifetime,
 * Is global only because it needs to be freed at the end of program, so the signal handler for shutdown has access to it
 */
SSL_CTX* ssl_ctx;

/**
 * Handler function for program shutdown, frees network
 * resources and ends successfully program with return code OK
 * @param signum Signal number
 */
void shutdown_signal_handler(int signum);

/**
 * Main program function
 * @param argc number of arguments
 * @param argv list of arguments
 * @return One of error codes defined in errors.h, OK if program ends successfully
 */
int main (int argc, char *argv[]);

#endif //ISA_MAIN_H
