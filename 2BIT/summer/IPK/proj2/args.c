/**
 *	@file	args.c
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Implementation of proccessing of arguments
 *	@note	Compiler: gcc 7.5
 */

#include "args.h"
#include "errors.h"
#include "sniffer.h"

void print_help(){
    printf("sudo ./ipk-sniffer -i interface [-p port] [--tcp|-t] [--udp|-u] [-n num] [--help|-h]\n");
    printf(" - program has to be run with admin privileges\n");
    printf(" - if no arguments is passed or when -i without argument is passed, lists available devices\n");
    printf("Arguments: \n");
    printf(" -i interface    - interface will be used for packet sniffing\n");
    printf(" -p port         - if set only packets that have source or destination port same as argument port\n");
    printf(" --tcp|-t        - if set only TCP packets will be caught\n");
    printf(" --udp|-u        - if set only UPD packets will be caught\n");
    printf(" -n num          - if set num number of packets will be caught, default 1\n");
    printf(" --help|-h       - prints help text to stdout\n");
}

char* copy_string_dynamic(char* s){
    unsigned short len = strlen(s);
    char* temp = calloc(len+1, sizeof(char));
    if (temp == NULL){
        fprintf(stderr, "Error internal, allocation of memory\n");
        return NULL;
    }
    memcpy(temp, optarg, len);
    return temp;
}

bool is_valid_integer(char* n){
    unsigned short len = strlen(n);
    for (unsigned short i = 0; i < len; i++){
        if (!isdigit(n[i])){
            return false;
        }
    }
    return true;
}

int process_args(int argc, char *argv[], char **interface, int *mode, int *num, char **port) {
    int valid_args = 0;
    bool udp = false;
    bool tcp = false;
    int c;
    static struct option long_opts[] =
        {
            {"tcp", no_argument, 0, 't'},
            {"udp", no_argument, 0, 'u'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "htui:p:n:", long_opts, &option_index)) != -1){
        switch (c) {
            case 't':
                tcp = true;
                valid_args++;
                break;
            case 'u':
                udp = true;
                valid_args++;
                break;
            case 'i':
                *interface = copy_string_dynamic(optarg);
                if (*interface == NULL){
                    return ERR_ARGS;
                }
                valid_args += 2;
                break;
            case 'p':
                if (is_valid_integer(optarg)){
                    int port_integer = atoi(optarg);
                    if (port_integer < 0 || port_integer > 65535){
                        fprintf(stderr, "Error port passed as argument outside of valid range 0 to 65535\n");
                        return ERR_ARGS;
                    }
                    *port = copy_string_dynamic(optarg);
                } else {
                    fprintf(stderr, "Error num argument for -n option must be integer, is: %s\n",optarg);
                    return ERR_ARGS;
                }
                valid_args += 2;
                break;
            case 'n':
                if (is_valid_integer(optarg)){
                    *num = atoi(optarg);
                } else {
                    fprintf(stderr, "Error num argument for -n option must be integer, is: %s\n", optarg);
                    return ERR_ARGS;
                }
                valid_args += 2;
                break;
            case 'h':
                print_help();
                if (*interface != NULL){
                    free(*interface);
                }
                return END_HELP;
            case '?':
                if (optopt == 'i'){
                    printf("Available devices: \n");
                    valid_args++;
                    break;
                } else {
                    return ERR_ARGS;
                }
            default:
                fprintf(stderr, "Internal error getopts\n");
                return ERR_ARGS;
        }
    }
    // setting the mode
    if (udp && tcp){
        *mode = MODE_BOTH;
    } else if (udp == true){
        *mode = MODE_UDP;
    } else if (tcp == true) {
        *mode = MODE_TCP;
    }

    // checking for any non option arguments
    if (argc-1 != valid_args){
        fprintf(stderr, "Error unrecognized arguments passed to the program\n");
        return ERR_ARGS;
    }
    return OK;
}