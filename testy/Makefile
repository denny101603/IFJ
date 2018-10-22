# Makefile
# IFJ projekt
# Autor: Daniel Bubeníček, FIT

CFLAGS =-std=c99 -pedantic -Wall -Wextra 
CC = gcc

all: scanner_test

main: scanner_test.o fsm.o
	$(CC) $(CFLAGS) scannet_test.o fsm.o -o scanner_test

scanner_test.o: scanner_test.c
	$(CC) $(CFLAGS) -c scanner_test.c -o scanner_test.o

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c -o fsm.o

clean:
	rm -f *.o scanner_test
