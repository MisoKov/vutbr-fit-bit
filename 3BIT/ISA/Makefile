# Makefile
# ISA projekt - Discord bot
# Michal Koval - xkoval17, FIT VUT
# 14.11.2020

# NOTE : use gmake in case of no input files error which may occur on eva.fit.vutbr.cz
#        fill in token
C=c++
CFLAGS=-Wno-deprecated-declarations -Wall -pedantic -std=c++11 -Wextra -Werror

all: isabot

argparser.o: argparser.cpp argparser.hpp errors.hpp
	$(C) $(CFLAGS) $< -c -o $@
isabot.o: main.cpp main.hpp errors.hpp
	$(C) $(CFLAGS) $< -c -o $@
primitive_json.o: primitive_json.cpp primitive_json.hpp errors.hpp
	$(C) $(CFLAGS) $< -c -o $@
network.o: network.cpp network.hpp errors.hpp
	$(C) $(CFLAGS) $< -c -o $@
isabot: isabot.o argparser.o network.o primitive_json.o
	$(C) $(CFLAGS) $^ -lssl -lcrypto -o $@

# only used during testing on my server
run:
	./isabot -t token

tar:
	tar -czvf xkoval17.tar *.cpp *.hpp Makefile README manual.pdf

clean:
	rm *.o isabot *.tar
