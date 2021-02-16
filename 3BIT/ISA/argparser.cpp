/**
 *	@file	argparsed.cpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Implementation of parsing of program arguments
 *	@note	Compiler: gcc 7.5
 */

#include "argparser.hpp"

using namespace std;

Args handle_args(int argc, char *argv[]){
    // Declaring and initializing return variable
    Args args;
    args.help = false;
    args.verbose = false;
    args.ret_code = OK;
    args.token.assign("");
    // Checking the count of arguments
    // No argument == printing help
    if (argc == 1){
        args.help = true;
    // No reason for more than 4 arguments at the same time
    } else if (argc > 5){
        cerr << "Too many arguments passed to program, read --help for program usage" << endl;
        args.ret_code = ERR_ARGS;
        return args;
    }
    // Checking the list of arguments
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-v") == 0 or strcmp(argv[i], "--verbose") == 0){
            if (args.verbose){
                cerr << "Verbose toggled more than once" << endl;
                args.ret_code = ERR_ARGS;
                return args;
            }
            args.verbose = true;
        } else if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0){
            if (args.help){
                cerr << "Help option used more than once" << endl;
                args.ret_code = ERR_ARGS;
                return args;
            }
            args.help = true;
        } else if (strcmp(argv[i], "-t") == 0){
            if (i + 1 > argc - 1){
                cerr << "Option -t is missing a parameter, it is required" << endl;
                args.ret_code = ERR_ARGS;
                return args;
            }
            i++;
            args.token.assign(argv[i]);
        } else {
            cerr << "Unknown option passed to the program" << endl;
            args.ret_code = ERR_ARGS;
            return args;
        }
    }
    // Printing of help message if it was set by options
    if (args.help){
        print_help();
        return args;
    }
    // Check if token was entered
    if (args.token.empty()){
        cerr << "Option -t was not entered, it is required" << endl;
        args.ret_code = ERR_ARGS;
        return args;
    }
    return args;
}

void print_help(){
    cout << "Usage: isabot [-h|--help] [-v|--verbose] -t <bot_access_token>" << endl;
    cout << "Options:" << endl;
    cout << "   -h|--help : prints help message to standard output" << endl;
    cout << "   -v|--verbose : prints the messages that bot reacts to to standard output formatted as \"<channel> - <username>: <message>\"" << endl;
    cout << "   -t <bot_access_token> : required token for authentication of bots access to Discord" << endl;
    cout << endl << "Help message is also printed when no option is entered." << endl;
}