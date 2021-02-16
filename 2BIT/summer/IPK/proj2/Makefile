# Makefile
# IPK project 2 - Packet Sniffer
# Michal Koval - xkoval17, FIT VUT
# 29.4.2020

C=gcc
CFLAGS=-std=c99 -pedantic -Wall -D_GNU_SOURCE

all: sniffer

sniffer.o: sniffer.c sniffer.h errors.h
	$(C) $(CFLAGS) $< -c -o $@

args.o: args.c args.h sniffer.h errors.h
	$(C) $(CFLAGS) $< -c -o $@

sniffer: main.c args.o sniffer.o
	$(C) $(CFLAGS) $^ -o $@ -lpcap

run:
	sudo ./sniffer

clean:
	rm sniffer.o args.o sniffer

tar:
	tar -cf xkoval17.tar *.c *.h README manual.pdf Makefile