/**
 *	@file	primitive_json.c
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Implementation of simple json parsing useful only to for use case of this project
 *	@note	Compiler: gcc 7.5
 */

#include "primitive_json.hpp"

using namespace std;

vector<stringstream*> parse_json_array(string json_text){
    // Initialization of variables needed for parsing
    vector<stringstream*> ret;
    char c;
    States state = OUTSIDE_BRACES;
    uint level = 0;
    uint el_idx = 0;

    // Going through json character by character
    for (size_t i = 0; i < json_text.size(); i++){
        c = json_text[i];
        if (state == OUTSIDE_BRACES){
            if (c == '{'){
                // New element
                state = INSIDE_BRACES;
                ret.push_back(new stringstream());
            }
        } else if (state == INSIDE_BRACES){
            if (c == '}'){
                if (level == 0) {
                    // End of element
                    el_idx += 1;
                    state = OUTSIDE_BRACES;
                } else {
                    // End of array inside element
                    level--;
                    *ret[el_idx] << c;
                }
            } else {
                // Beginning of array inside element
                if (c == '{'){
                    level++;
                }
                *ret[el_idx] << c;
            }
        } else {
            // Unreachable state
            return ret;
        }
    }
    return ret;
}

string parse_json_body(string response){
    regex reg(JSON_REG);
    smatch matches;
    regex_search(response, matches, reg);
    if (matches.size() > 0){
        return matches.str();
    } else {
        return "";
    }
}

string parse_with_regex(string response, const char* reg_s){
    regex reg(reg_s);
    smatch matches;
    regex_search(response, matches, reg);
    if (matches.size() > 0){
        return matches[1].str();
    } else {
        return "";
    }
}

int get_reset_time(string response){
    // Parsing x-ratelimit-remaining from response
    string parsed;
    parsed = parse_with_regex(response, LIMIT_REMAINING_REG);
    if (parsed.empty()) {
        return FAIL;
    }
    int rem = stof(parsed);
    if (rem > 0){
        // There are remaining messages in x-rate window so no sleep is needed
        return OK;
    }
    // Parsing x-ratelimit-reset time from response
    parsed = parse_with_regex(response, RESET_REG);
    if (parsed.empty()) {
        return FAIL;
    }
    return stoi(parsed)+1;
}