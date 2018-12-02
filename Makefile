# Makefile
# IFJ projekt
# Autor: Daniel Bubeníček, FIT

CFLAGS =-std=c99 -pedantic -Wall -Wextra
CC = gcc

all: main

main: main.o scanner.o parser_expr.o parser.o symtable.o tac.o code_gen.o garbage_collector.o
	$(CC) $(CFLAGS) main.o scanner.o parser_expr.o parser.o symtable.o tac.o code_gen.o garbage_collector.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

parser_expr.o: parser_expr.c parser_expr.h
	$(CC) $(CFLAGS) -c parser_expr.c -o parser_expr.o

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

tac.o: tac.c tac.h
	$(CC) $(CFLAGS) -c tac.c -o tac.o

code_gen.o: code_gen.c code_gen.h
	$(CC) $(CFLAGS) -c code_gen.c -o code_gen.o

garbage_collector.o: garbage_collector.c garbage_collector.h
	$(CC) $(CFLAGS) -c garbage_collector.c -o garbage_collector.o

clean:
	rm -f *.o main

test:
    python3 test.py --fast=true