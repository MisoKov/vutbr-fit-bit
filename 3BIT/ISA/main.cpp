/**
 *	@file	main.cpp
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	14.11.2020
 *	@brief	Main file implementation of Discord bot
 *	@note	Compiler: gcc 7.5
 */

#include "main.hpp"

using namespace std;

void shutdown_signal_handler(int signum){
    cout << endl << "Signal :" << signum << " - Terminating program" << endl;
    end_of_program_clear(ssl_ctx);
    exit(OK);
}

int main (int argc, char *argv[]){
    // Processing of arguments
    Args args = handle_args(argc, argv);
    if (args.ret_code == ERR_ARGS){
        return ERR_ARGS;
    } else if (args.help){
        return OK;
    }

    // Registering signal handler for turning off of program
    signal(SIGINT, shutdown_signal_handler);

    // Creating SSL_CTX
    SSL_library_init();
    if ((ssl_ctx = create_CTX()) == NULL){
        return ERR_SSL;
    }

    // Creating connection to Discord
    SSL_conn ssl_conn = setup_connection(ssl_ctx);

    // Getting guild ID
    Guild_info guild_info = get_guild_ID(ssl_conn.ssl_bio, args.token);
    if (guild_info.ret_code != OK){
        network_clear(ssl_conn);
        end_of_program_clear(ssl_ctx);
        return guild_info.ret_code;
    }

    // Getting the #isa-bot channel ID
    Channel_info channel_info = get_channel(ssl_conn.ssl_bio, args.token, guild_info.guild_ID, guild_info.reset_time);
    if (channel_info.ret_code != OK){
        network_clear(ssl_conn);
        end_of_program_clear(ssl_ctx);
        return channel_info.ret_code;
    }

    network_clear(ssl_conn);

    // Listening to new messages and sending their copies
    New_msgs new_msgs;
    int time = 0;
    pair<int, int> msg_ret;
    string last_ID = channel_info.last_msg_ID;
    queue<Msg_info> msgs_to_send;
    while (true) {
        // Get new messages since last message
        new_msgs = get_channel_messages(ssl_ctx, args.token, channel_info.channel_ID, last_ID, time);
        time = new_msgs.reset_time;
        if (new_msgs.msg_list.size() > 0){
            last_ID = new_msgs.msg_list.back().msg_ID;
            for (Msg_info x : new_msgs.msg_list) {
                msgs_to_send.push(x);
            }
        } else {
            if (msgs_to_send.size() == 0){
                // Sleep for second until next check of new messages
                // Required for handling of rate limiting and smooth user experience
                sleep(1);
                continue;
            }
        }
        // Reply to every new non bot message
        while (true) {
            if (msgs_to_send.size() == 0){
                break;
            }
            Msg_info x = msgs_to_send.front();
            msg_ret = send_msg(ssl_ctx, x.content, x.username, args.token, channel_info.channel_ID, time);
            if (msg_ret.first != OK){
                break;
            }
            time = msg_ret.second;
            // Print to stdout if verbose is toggled
            if (args.verbose){
                cout << "isa-bot - " << x.username << " : " << x.content << endl;
            }
            msgs_to_send.pop();
        }
    }
    return OK;
}