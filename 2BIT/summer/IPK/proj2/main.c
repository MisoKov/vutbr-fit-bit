/**
 *	@file	sniffer.c
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Implementation of sniffer part of project
 *	@note	Compiler: gcc 7.5
 */

#include "sniffer.h"
#include "errors.h"
#include "args.h"

int main(int argc, char *argv[]) {
    int mode = MODE_BOTH;
    int num_packets = 1;
    char *port = NULL;
    char *interface = NULL;
    int ret;
    // proccessing of arguments
    if ((ret = process_args(argc, argv, &interface, &mode, &num_packets, &port)) != OK){
        return ret;
    }
    // check if -i was set
    if (interface == NULL) {
        return print_interfaces();
    }
    if ((ret = start_sniffing(interface, mode, num_packets, port)) != OK){
        free(interface);
        return ret;
    }
    free(interface);
    return OK;
}