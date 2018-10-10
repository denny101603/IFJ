# Makefile
# IFJ projekt
# Autor: Daniel Bubeníček, FIT

CFLAGS =-std=c99 -pedantic -Wall -Wextra
CC = gcc

all: main

main: main.o fsm.o
	$(CC) $(CFLAGS) -o main main.o fsm.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c -o fsm.o
