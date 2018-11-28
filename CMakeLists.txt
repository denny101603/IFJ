cmake_minimum_required(VERSION 3.12)
project(IFJ2018 C)

set(CMAKE_C_STANDARD 99)

add_executable(IFJ2018 main.c err_codes.h fsm.c fsm.h savo.c savo.h sax.c sax.h symtable.c symtable.h seman.h seman.c code_gen.c code_gen.h garbage_collector.c garbage_collector.h)