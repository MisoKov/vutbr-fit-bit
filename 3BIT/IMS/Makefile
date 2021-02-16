# Makefile
# IMS projekt - Epidemiologické modely pomocí celulárních automatů
# Michal Koval - xkoval17, FIT VUT
# 1.12.2020

C=c++
CFLAGS=-Wall -pedantic -std=c++11 -Wextra #-Werror

all: sim

cell.o: cell.cpp cell.hpp
	$(C) $(CFLAGS) $< -c -o $@
pca.o: pca.cpp pca.hpp
	$(C) $(CFLAGS) $< -c -o $@
sim.o: main.cpp main.hpp
	$(C) $(CFLAGS) $< -c -o $@
sim: main.o cell.o pca.o
	$(C) $(CFLAGS) $^  -o $@

run:
	./sim

tar:
	tar -czvf xkoval17.tar *.cpp *.hpp Makefile xkoval17.pdf

clean:
	rm *.o sim *.tar