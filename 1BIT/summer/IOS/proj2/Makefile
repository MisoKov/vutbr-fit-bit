# Makefile pre IOS projekt 2: River Crossing Problem
# author: Michal Koval, xkoval17, FIT VUT
# date: 18.04.2019

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

# -----------------------------------------------
all: proj2

pack:
	zip proj2.zip *.h *.c Makefile

clean:
	rm -rf *.o proj2 *.zip
# -----------------------------------------------
proj2.o: proj2.c proj2.h
	$(CC) $(CFLAGS) -c proj2.c -o $@ -lpthread -lrt
proj2: proj2.o
	$(CC) $(CFLAGS) proj2.o -o $@ -lpthread -lrt