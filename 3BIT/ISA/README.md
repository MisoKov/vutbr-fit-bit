# [ISA - Network Applications and Network Administration](https://www.fit.vut.cz/study/course/14020/)
## Discord Bot - Message pinger
**Obtained points**:<br>
18/20

Simple Discord bot, connects to "isa-bot" channel and pings every message whose author is not bot. 
Bots are recognized by having string "bot" in their username. Channel has to be premade and bot has to also
manually created beforehand. 

**IMPORTANT**:
Bot needs to be first manually added to server with permissins:<br>
* View Channels
* Embed Links
* Read Message History

**IMPORTANT**: The host has to make at least one connection to Discord Gateway for the bot to be permitted to send messages.
For that use the gateway.html with your token.

### Compile:
    make
Note: on FreeBSD (eva.fit.vutbr.cz) use : `gmake`

### Run:
    ./isabot [-h|--help] [-v|--verbose] -t <bot_access_token>
### Parameters:
    -h|--help : prints help message to standard output
    -v|--verbose : prints the messages that bot reacts to to standard output formatted as "<channel> - <username>: <message>"
    -t <bot_access_token> : required token for authentication of bots access to Discord
    Help message is also printed when no option is entered
    
### Examples of running:
    ./isabot -t NzY3NDc1MDM5NTMxOTU4Mjgy.X4yc1A.J0YK0VPZBX1I1y7-0bSnlCNiYj8
    ./isabot -t NzY3NDc1MDM5NTMxOTU4Mjgy.X4yc1A.J0YK0VPZBX1I1y7-0bSnlCNiYj8 --verbose


### File list: 
* Makefile
* main.cpp
* main.hpp
* network.cpp
* network.hpp
* primitive_json.cpp
* primitive_json.hpp
* argparser.cpp
* argparser.hpp
* errors.hpp
* manual.pdf
* README
