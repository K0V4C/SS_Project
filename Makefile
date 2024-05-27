CC = g++
LEXER = flex
BISON = bison

PFLAGS = -d -t
CFLAGS = -Ifl -Iinc/asembler/asm_actions -I/inc/common -I. -std=c++17

BISON_FILE = misc/bison_gen.ypp
FLEX_FILE = misc/flex_gen.lpp

SOURCES = $(wildcard *.cpp) $(wildcard *.c) $(wildcard src/asembler/*.cpp) $(wildcard src/common/*.cpp) $(wildcard src/asembler/asm_actions/*.cpp) 

bison_gen.tab.cpp bison_gen.tab.hpp: $(BISON_FILE)
	$(BISON) $(PFLAGS) $(BISON_FILE)

lex.yy.c: $(FLEX_FILE) bison_gen.tab.hpp 
	$(LEXER) $(FLEX_FILE)

asembler: bison_gen.tab.cpp bison_gen.tab.hpp lex.yy.c
	$(CC) $(CFLAGS) $(SOURCES) -o asembler.out


clean:
	rm -f asembler.out  
	rm -f bison_gen.tab.cpp bison_gen.tab.hpp
	rm -f lex.yy.c
	rm -f *.o
