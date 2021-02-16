/**
 *	@file	network.cpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Implements functions that have anything to do with network part of program, connection establishment, http..
 *	@note	Compiler: gcc 7.5
 */

#include "network.hpp"

using namespace std;

void init_ssl(){
    // Initializing SSL library
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

void network_clear(SSL_conn ssl_conn){
    //SSL_free(ssl_conn.ssl);
    BIO_free (ssl_conn.connect_bio);
    BIO_free (ssl_conn.ssl_bio);
}

void end_of_program_clear(SSL_CTX* ctx) {
    EVP_cleanup();
    SSL_CTX_free(ctx);
    ERR_free_strings();
}

void handle_error(const char* err_msg){
    // Printing error messages
    cerr << err_msg << endl;
    ERR_print_errors_fp (stderr);
}

SSL_CTX* create_CTX(void){
    // Creating SSL_CTX using TSL 1.2, stated in Discord documentation
    SSL_CTX* ctx;
    ctx = SSL_CTX_new(TLSv1_2_method());
    if (ctx == NULL){
        handle_error("Failed to create SSL_CTX");
        return NULL;
    }
    return ctx;
}

SSL_conn setup_connection(SSL_CTX* ctx){
    SSL_conn ret;
    ret.ssl_bio = NULL;
    ret.connect_bio = NULL;
    ret.ssl = NULL;
    // Creating SSL struct
    SSL* ssl = SSL_new (ctx);
    if (ssl == NULL){
        handle_error ("Error: Failed to create new SSL structure");
        return ret;
    }
    SSL_set_connect_state (ssl);

    // Creating BIO for connection with ssl struct
    BIO* ssl_bio = BIO_new (BIO_f_ssl ());
    if (ssl_bio == NULL){
        handle_error ("Error: Failed creating SSL BIO");
        SSL_free(ssl);
        return ret;
    }
    BIO_set_ssl (ssl_bio, ssl, BIO_CLOSE);

    // Creating BIO for connection with discord
    BIO* bio = BIO_new_connect (DISCORD_URL_PORT);
    if (bio == NULL){
        handle_error ("Error: Failed creating connect BIO");
        BIO_free (ssl_bio);
        SSL_free(ssl);
        return ret;
    }
    if (BIO_do_connect (bio) <= 0){
        handle_error ("Error: Failed to connect to discord.com");
        BIO_free (bio);
        SSL_free(ssl);
        BIO_free (ssl_bio);
        return ret;
    }

    // Chain BIOs
    BIO_push (ssl_bio, bio);

    ret.ssl_bio = ssl_bio;
    ret.connect_bio = bio;
    ret.ssl = ssl;
    return ret;
}

pair<int,string> send_using_ssl(const char* message, BIO* ssl_bio, int sleep_time){
    // Preparing return pair
    pair<int,string> ret;
    ret.second = "";

    // Sleep if x-rate-reset has not yet passed
    auto now = std::chrono::system_clock::now().time_since_epoch();
    int diff = sleep_time - std::chrono::duration_cast<std::chrono::seconds>(now).count();
    if (diff > 0){
        this_thread::sleep_until(chrono::system_clock::now() + chrono::seconds(diff));
    }

    // Sending the request
    if (BIO_puts (ssl_bio, message) <= 0){
        handle_error ("Error: Failed to send request");
        ret.first = ERR_SSL;
        return ret;
    }

    // Receiving response
    char buf[4096];
    stringstream response;
    int len;
    while (true) {
        len = BIO_read (ssl_bio, buf, sizeof (buf));
        if (len < 10){
            break;
        } else {
            // Putting together chunked response
            for (int i = 0; i < len; i++){
                response << buf[i];
            }
            // Check for Bad request so the communication doesnt cycle forever
            if (response.str().find("HTTP/1.1 4") != string::npos){
                ret.first = ERR_HTTP;
                ret.second = response.str();
                return ret;
            }
        }
    }

    // Setting return variable
    ret.first = OK;
    ret.second = response.str();

    return ret;
}

Guild_info get_guild_ID(BIO* ssl_bio, string bot_token){
    Guild_info ret;
    ret.guild_ID = "";
    // Preparing request
    stringstream ss;
    ss << "GET /api/v8/users/@me/guilds HTTP/1.1" << "\r\n"
       << "Host: discord.com\r\n"
       << "Accept: application/json\r\n"
       << "Connection: keep-alive\r\n"
       << "User-Agent: DiscordBot\r\n"
       << "Authorization: Bot " << bot_token
       << "\r\n\r\n";
    // Sending the GET request for guild information
    pair<int,string> response;
    response = send_using_ssl(ss.str().c_str(), ssl_bio, 0);
    if (response.first != OK){
        if (response.second.find("401 Unauthorized)")) {
            cerr << "Error invalid token: 401 Unauthorized response from Discord" << endl;
        }
        ret.ret_code = response.first;
        return ret;
    }
    // Checking the response code
    int http_res_c;
    http_res_c = check_response_code(response.second.c_str());
    if (http_res_c != SUCCESS){
        ret.ret_code = FAIL;
        return ret;
    }
    // Parsing reset time from response
    if ((ret.reset_time = get_reset_time(response.second)) == FAIL) {
        ret.ret_code = FAIL;
        return ret;
    }

    // Parsing the guild ID from response
    string parsed = parse_with_regex(response.second, ID_REG);
    if (parsed.empty()){
        ret.ret_code = ERR_HTTP;
    } else {
        ret.guild_ID = parsed;
        ret.ret_code = OK;
    }

    return ret;
}

Channel_info get_channel(BIO* ssl_bio, string bot_token, string guild_ID, int sleep_time){
    Channel_info ret;
    ret.channel_ID = "";
    ret.last_msg_ID = "";
    // Preparing request
    stringstream ss;
    ss << "GET /api/v8/guilds/"<< guild_ID <<"/channels HTTP/1.1" << "\r\n"
       << "Host: discord.com\r\n"
       << "Accept: application/json\r\n"
       << "Connection: keep-alive\r\n"
       << "User-Agent: DiscordBot\r\n"
       << "Authorization: Bot " << bot_token
       << "\r\n\r\n";
    // Sending GET request for list of channels in the guild
    pair<int,string> response;
    response = send_using_ssl(ss.str().c_str(), ssl_bio, sleep_time);
    if (response.first != OK){
        ret.ret_code = response.first;
        return ret;
    }
    // Checking the response code
    int http_res_c;
    http_res_c = check_response_code(response.second.c_str());
    if (http_res_c != SUCCESS){
        ret.ret_code = FAIL;
        return ret;
    }
    // Parsing the guild body containing json from response
    string parsed = parse_json_body(response.second);
    if (parsed.empty()){
        ret.ret_code = ERR_HTTP;
        return ret;
    }
    // Looking for isa-bot channel
    vector<stringstream*> parsed_json = parse_json_array(parsed);
    for (uint i = 0; i < parsed_json.size(); i++){
        if (parsed_json[i]->str().find(CHANNEL_NAME) != string::npos){
            // Getting the channel ID
            string ch_id = parse_with_regex(parsed_json[i]->str(), ID_REG);
            if (ch_id.empty()){
                for (stringstream* x : parsed_json){
                    delete(x);
                }
                ret.ret_code = ERR_HTTP;
                return ret;
            }
            // Getting the last message ID
            string last_msg = parse_with_regex(parsed_json[i]->str(), LAST_MSG_REG);
            if (last_msg.empty()){
                for (stringstream* x : parsed_json){
                    delete(x);
                }
                ret.ret_code = ERR_HTTP;
                return ret;
            }
            if (last_msg == "null"){
                last_msg = "";
            } else {
                last_msg = last_msg.substr(1, last_msg.length()-2);
            }

            // Returning successfully
            ret.channel_ID = ch_id;
            ret.last_msg_ID = last_msg;
            ret.ret_code = OK;
            for (stringstream* x : parsed_json){
                delete(x);
            }
            return ret;
        }
    }
    // Channel isa-bot was not found in guild channels
    for (stringstream* x : parsed_json){
        delete(x);
    }
    ret.ret_code = FAIL;
    return ret;
}

New_msgs get_channel_messages(SSL_CTX* ctx, string bot_token, string channel_ID, string last_msg_ID, int reset_time){
    New_msgs ret;
    // Preparing request
    stringstream ss;
    if (last_msg_ID.empty()){
        ss << "GET /api/v8/channels/" << channel_ID << "/messages HTTP/1.1" << "\r\n";
    } else {
        ss << "GET /api/v8/channels/" << channel_ID << "/messages?after=" << last_msg_ID << " HTTP/1.1" << "\r\n";
    }
    ss << "Host: discord.com\r\n"
       << "Accept: application/json\r\n"
       << "Connection: close\r\n"
       << "Content-Type: application/json\r\n"
       << "User-Agent: DiscordBot\r\n"
       << "Authorization: Bot " << bot_token
       << "\r\n\r\n";
    // Create new connection
    SSL_conn ssl_conn = setup_connection(ctx);

    // Sending GET request for list of channels in the guild
    pair<int,string> response;
    response = send_using_ssl(ss.str().c_str(), ssl_conn.ssl_bio, reset_time);
    if (response.first != OK){
        ret.ret_code = response.first;
        return ret;
    }

    network_clear(ssl_conn);

    // Checking the response code
    int http_res_c;
    http_res_c = check_response_code(response.second.c_str());
    if (http_res_c != SUCCESS){
        ret.ret_code = FAIL;
        return ret;
    }

    // Parsing reset time from response
    if ((ret.reset_time = get_reset_time(response.second)) == FAIL) {
        ret.ret_code = FAIL;
        return ret;
    }

    // Parsing the guild body containing json from response
    string json = parse_json_body(response.second);
    if (json.empty()){
        ret.ret_code = ERR_HTTP;
        return ret;
    }

    // Going through all messages and separating messages from bots
    vector<stringstream*> parsed_json = parse_json_array(json);
    for (uint i = 0; i < parsed_json.size(); i++){
        string username = parse_with_regex(parsed_json[i]->str(), USERNAME_REG);
        if (username.find("bot") == string::npos){
            string content = parse_with_regex(parsed_json[i]->str(), CONTENT_REG);
            string id = parse_with_regex(parsed_json[i]->str(), ID_REG);
            Msg_info msg;
            msg.content = content;
            msg.username = username;
            msg.msg_ID = id;
            ret.msg_list.push_back(msg);
        }
    }
    for (stringstream* x : parsed_json){
        delete(x);
    }

    if (ret.msg_list.size() > 1){
        reverse(ret.msg_list.begin(), ret.msg_list.end());
    }

    ret.ret_code = OK;
    return ret;
}


pair<int,int> send_msg(SSL_CTX* ssl_ctx, string msg, string username, string bot_token, string channel_ID, int reset_time){
    pair<int,int> ret;
    ret.first = OK;
    ret.second = 0;
    // Preparing request
    stringstream request;
    request << "POST /api/v8/channels/" << channel_ID << "/messages HTTP/1.1" << "\r\n"
           << "Host: discord.com\r\n"
           << "Content-Type: application/json\r\n"
           << "Connection: close\r\n"
           << "User-Agent: DiscordBot\r\n"
           << "Authorization: Bot " << bot_token << "\r\n";
    stringstream body;
    body << "{\"content\": \"echo: " << username << " - " << msg << "\", \"tts\": \"false\"}";
    request << "Content-Length: " << body.str().size()
            << "\r\n\r\n"
            << body.str();

    // Create new connection
    SSL_conn ssl_conn = setup_connection(ssl_ctx);

    // Sending GET request for list of channels in the guild
    pair<int,string> response;
    response = send_using_ssl(request.str().c_str(), ssl_conn.ssl_bio, reset_time);
    if (response.first != OK){
        network_clear(ssl_conn);
        ret.first = response.first;
        return ret;
    }

    network_clear(ssl_conn);

    // Checking the response code
    int http_res_c;
    http_res_c = check_response_code(response.second.c_str());
    if (http_res_c != SUCCESS){
        ret.first = FAIL;
        return ret;
    }

    // Parsing reset time from response
    int time;
    if ((time = get_reset_time(response.second)) == FAIL) {
        ret.first = FAIL;
        return ret;
    }

    return ret;
}

int check_response_code(const char* response){
    stringstream ss;
    ss << response;
    string line;
    getline(ss, line);
    if (line.find("HTTP/1.1 1") != string::npos){
        return INFO;
    } else if (line.find("HTTP/1.1 2") != string::npos){
        return SUCCESS;
    } else if (line.find("HTTP/1.1 3") != string::npos){
        return REDIRECT;
    } else if (line.find("HTTP/1.1 4") != string::npos){
        return C_ERR;
    } else if (line.find("HTTP/1.1 5") != string::npos){
        return S_ERR;
    } else {
        return FAIL;
    }
}

