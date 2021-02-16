/**
 *	@file	network.hpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Header file for network implementation part of program
 *	@note	Compiler: gcc 7.5
 */

#ifndef ISA_NETWORK_H
#define ISA_NETWORK_H

#include "errors.hpp"
#include "primitive_json.hpp"
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <chrono>
#include <thread>

/**
 * Constants
 */
#define DISCORD_URL_PORT "discord.com:https"
#define CHANNEL_NAME "isa-bot"

/**
 * HTTP response return codes
 */
#define INFO 1
#define SUCCESS 2
#define REDIRECT 3
#define C_ERR 4
#define S_ERR 5

using namespace std;

// ----------  STRUCTURES ----------
/**
 * Structure used to store and return from functions required channel information
 */
typedef struct {
    int ret_code;
    string channel_ID;
    string last_msg_ID;
} Channel_info;

/**
 * Structure used to store and return from functions required guild information
 */
typedef struct {
    int ret_code;
    string guild_ID;
    int reset_time;
} Guild_info;

/**
 * Structure used to store and return from functions required message information
 */
typedef struct {
    string username;
    string content;
    string msg_ID;
} Msg_info;

/**
 * Structure used to return data from get_channel_messages
 */
typedef struct {
    int ret_code;
    vector<Msg_info> msg_list;
    int reset_time;
} New_msgs;

/**
 * Structure containing SSL connection variables, returned from setup_connection
 */
typedef struct {
    BIO* ssl_bio;
    BIO* connect_bio;
    SSL* ssl;
} SSL_conn;

// ----------  FUNCTIONS ----------
/**
 * Initializing SSL library
 */
void init_ssl();

/**
 * Creates new SSL_CTX which will be used throughout the whole program life.
 * Must be freed with network_clear(ssl_ctx) function
 * @return Pointer to newly created SSL_CTX struct
 */
SSL_CTX* create_CTX();

/**
 * Used to free SSL_conn structure
 * Must be called before closing the program.
 * @param ssl_conn SSL_conn struct which will be freed
 */
void network_clear(SSL_conn ssl_conn);

/**
 * Gets channel ID of isa-bot channel and ID of last message send to the channel
 * @param ssl_bio BIO which will be used to sent http request, must be properly set up beforehand
 * @param bot_token authentication token of bot
 * @param guild_ID string containing server/guild ID
 * @param sleep_time int of seconds which represent time when new request can be sent, 0 if no waiting is needed
 * @return Channel_info struct containing parsed data from http response
 *         and ret_code which will be OK if no error occured, or one of error codes defined in errors.h
 */
Channel_info get_channel(BIO* ssl_bio, string bot_token, string guild_ID, int sleep_time);

/**
 *
 * @param ssl_bio BIO which will be used to sent http request, must be properly set up beforehand
 * @param bot_token authentication token of bot
 * @return Guild_info struct containing parsed data from http response
 *         and ret_code which will be OK if no error occured, or one of error codes defined in errors.h
 */
Guild_info get_guild_ID(BIO* ssl_bio, string bot_token);

/**
 * Checks the response code of message
 * @param response const char array containing reponse text
 * @return int one of defined response codes
 */
int check_response_code(const char* response);

/**
 * Prints errors to stderr
 * @param err_msg error message which will be printed
 */
void handle_error(const char* err_msg);

/**
 * Sends http request using SSL encryption
 * Waits before sending next message if its needed according to entered reset_time
 * @param message cont char array containing message which will be send, has to valid request
 * @param bio_ssl BIO which will be used to sent http request, must be properly set up beforehand
 * @return pair of int containing return code, and string containing response
 */
pair<int,string> send_using_ssl(const char* message, BIO* bio_ssl, int reset_time);

/**
 * Gets the messages from the channel the were send since last_msg_ID
 * @param ssl_ctx SSL context, needed to create new connection
 * @param bot_token authentication token of bot
 * @param channel_ID string containing ID of channel from which the messages will be received
 * @return New_msgs struct containing list of Msg_info for new messages since last_msg_ID, can be empty if none new
 *         messages came, return code ret_code and
 */
New_msgs get_channel_messages(SSL_CTX* ctx, string bot_token, string channel_ID, string last_msg_ID, int reset_time);


/**
 * Adds new message from bot to channel, in format of "echo: <username> - <message>"
 * @param ssl_ctx SSL context, needed to create new connection
 * @param msg string containing text of message
 * @param username string containing the name of author of message
 * @param bot_token authentication token of bot
 * @param channel_ID string containing ID of channel where the message will be send
 * @return pair of 2 ints, first is return code, second is time to wait until next request should be send
 */
pair<int,int> send_msg(SSL_CTX* ssl_ctx, string msg, string username, string bot_token, string channel_ID, int reset_time);

/**
 * Clears remaining network resources, needs to be called only once during proram lifetime, before shutdown
 * @param ctx pointer to SSL_CTX which will be freed
 */
void end_of_program_clear(SSL_CTX* ctx);


/**
 * Setups new SSL connection through use of BIOs
 * @param ctx SSL context of program
 * @return SSL_conn struct containing connection variables
 */
SSL_conn setup_connection(SSL_CTX* ctx);

#endif //ISA_NETWORK_H
