cmake_minimum_required(VERSION 3.12)
project(IFJ2018 C)

set(CMAKE_C_STANDARD 99)

add_executable(IFJ2018 main.c err_codes.h scanner.c scanner.h parser_expr.c parser_expr.h parser.c parser.h symtable.c symtable.h tac.h tac.c code_gen.c code_gen.h garbage_collector.c garbage_collector.h)