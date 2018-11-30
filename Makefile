# Makefile
# IFJ projekt
# Autor: Daniel Bubeníček, FIT

CFLAGS =-std=c99 -pedantic -Wall -Wextra
CC = gcc

all: main

main: main.o fsm.o savo.o sax.o symtable.o seman.o code_gen.o garbage_collector.o
	$(CC) $(CFLAGS) main.o fsm.o savo.o sax.o symtable.o seman.o code_gen.o garbage_collector.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c -o fsm.o

savo.o: savo.c savo.h
	$(CC) $(CFLAGS) -c savo.c -o savo.o

sax.o: sax.c sax.h
	$(CC) $(CFLAGS) -c sax.c -o sax.o

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

seman.o: seman.c seman.h
	$(CC) $(CFLAGS) -c seman.c -o seman.o

code_gen.o: code_gen.c code_gen.h
	$(CC) $(CFLAGS) -c code_gen.c -o code_gen.o

garbage_collector.o: garbage_collector.c garbage_collector.h
	$(CC) $(CFLAGS) -c garbage_collector.c -o garbage_collector.o

clean:
	rm -f *.o main