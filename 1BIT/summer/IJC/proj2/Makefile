# Makefile
# IJC projekt 2
# Michal Koval - xkoval17, FIT VUT
# 14.4.2019

C=gcc
CC=g++
CFLAGS=-std=c99 -pedantic -Wall -Wextra
CCFLAGS=-std=c++11 -pedantic -Wall
LD_LIBRARY_PATH=.
OPT=-O2
OBJ_LIB=htab_iterator_get_key.o htab_begin.o htab_bucket_count.o htab_clear.o htab_end.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_move.o htab_size.o htab_iterator_get_value.o htab_iterator_next.o htab_iterator_set_value.o

all: tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so

clean:
	rm tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so *.o
pack:
	zip xkoval17.zip *.c *.cc *.h Makefile
# ------------------- TAIL ----------------------
tail.o: tail.c tail.h
	$(C) $(CFLAGS) -c -o $@ $< -lm
tail: tail.o tail.c tail.h
	$(C) $(CFLAGS) -o $@ $< -lm
tail2.o: tail2.cc
	$(CC) $(CCFLAGS) -c -o $@ $<
tail2: tail2.o tail2.cc
	$(CC) $(CCFLAGS) -o $@ $<
# --------------- WORDCOUNT -----------------------
# ------------------ OBJ ------------------------
htab_begin.o: htab_begin.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_bucket_count.o: htab_bucket_count.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_clear.o: htab_clear.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_end.o: htab_end.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_free.o: htab_free.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_hash_function.o: htab_hash_function.c htab.h Makefile
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $< #-DHASHTEST
htab_init.o: htab_init.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_lookup_add.o: htab_lookup_add.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_move.o: htab_move.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_size.o: htab_size.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_iterator_get_value.o: htab_iterator_get_value.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_iterator_next.o: htab_iterator_next.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_iterator_set_value.o: htab_iterator_set_value.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
htab_iterator_get_key.o: htab_iterator_get_key.c htab.h
	$(C) $(CFLAGS) $(OPT) -c -fPIC -o $@ $<
io.o: io.c htab.h
	$(C) $(CFLAGS) $(OPT) -c $< -o $@
wordcount.o: wordcount.c htab.h Makefile
	$(C) $(CFLAGS) $(OPT) -c wordcount.c -o $@ #-DTEST
# ----------------- EXE ------------------
wordcount: wordcount.o htab.h libhtab.a io.o
	$(C) $(CFLAGS) $(OPT) -static -L$(LD_LIBRARY_PATH) wordcount.o libhtab.a io.o -o $@
wordcount-dynamic: wordcount.o io.o libhtab.so htab.h
	$(C) $(CFLAGS) $(OPT) -L$(LD_LIBRARY_PATH) -fPIC wordcount.o libhtab.a io.o -o $@
# ----------------- LIB --------------------
libhtab.a: $(OBJ_LIB)
	ar rcs $@ $^
libhtab.so: $(OBJ_LIB)
	$(C) -shared -fPIC $^ -L$(LD_LIBRARY_PATH) -o $@ 
