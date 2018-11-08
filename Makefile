# Makefile
# IFJ projekt
# Autor: Daniel Bubeníček, FIT

CFLAGS =-std=c99 -pedantic -Wall -Wextra
CC = gcc

all: main

main: main.o fsm.o savo.o sax.o
	$(CC) $(CFLAGS) main.o fsm.o savo.o sax.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c -o fsm.o

savo.o: savo.c savo.h
	$(CC) $(CFLAGS) -c savo.c -o savo.o

sax.o: sax.c sax.h
	$(CC) $(CFLAGS) -c sax.c -o sax.o

clean:
	rm -f *.o main