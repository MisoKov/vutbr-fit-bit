/**
 *	@file	primitive_json.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Header file for of json parsing part of program
 *	@note	Compiler: gcc 7.5
 */

#ifndef ISA_PRIMITIVE_JSON_H
#define ISA_PRIMITIVE_JSON_H

#include "errors.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

/**
 * Regexes for data parsing from responses
 */
#define JSON_REG "\\[.*\\]\\s"                                  // regex for body of response containing json
#define USERNAME_REG "\"username\": \"(.*?)\","                 // regex for username
#define CONTENT_REG "\"content\": \"(.*?)\","                   // regex for content
#define ID_REG "\"id\": \"(.*?)\","                             // regex for id
#define LAST_MSG_REG "\"last_message_id\": (.*?),"              // regex for last message ID
#define CH_NAME_REG "/\"name\": \"(.*?)\","                     // regex for channel name
#define RESET_REG "x-ratelimit-reset: (\\d+\\.*\\d+)"           // regex for x-ratelimit-reset
#define LIMIT_REMAINING_REG "x-ratelimit-remaining: (\\d+)"     // regex for x-ratelimit-remaining

using namespace std;

/**
 * Enumerate of states used to parse json array
 */
typedef enum {
    OUTSIDE_BRACES,
    INSIDE_BRACES
} States;

/**
 * Parses json array to vector of stringstreams, each representing one element of json array
 * Its basically very simple two states finite automata
 * @param json_text string of text in json format containing json array with multiple elements
 * @return vector of stringstreams, each containing one element of input json array
 */
vector<stringstream*> parse_json_array(string json_text);

/**
 * General parse function using regex, parses input response with input string containing regular expression
 * @param response string text containing http response
 * @param reg_s const char array containing regular expression used to match required substring form response
 * @return string containing match of regular expression or empty string
 */
string parse_with_regex(string response, const char* reg_s);

/**
 * Parses reset time from response when x-rate-remaining reaches 0
 * @param response string containing http response
 * @return integer representing time in seconds from epoch when remaining x rate reach 0,
 *         0 when we still have some left
 */
int get_reset_time(string response);

/**
 * Parses whole json body from http response
 * @param response string containing http response
 * @return string containing json from body of http reponse
 */
string parse_json_body(string response);

#endif //ISA_PRIMITIVE_JSON_H
