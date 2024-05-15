CC = g++
LEXER = flex
BISON = bison

PFLAGS = -d -t
CFLAGS = -Iinc -I. -std=c++17 -Ifl

BISON_FILE = misc/bison_gen.ypp
FLEX_FILE = misc/flex_gen.lpp

SOURCES = $(wildcard *.cpp) $(wildcard src/*.cpp) $(wildcard *.c)


bison_gen.tab.cpp bison_gen.tab.hpp: $(BISON_FILE)
	$(BISON) $(PFLAGS) $(BISON_FILE)

lex.yy.c: $(FLEX_FILE) bison_gen.tab.hpp 
	$(LEXER) $(FLEX_FILE)

create: bison_gen.tab.cpp bison_gen.tab.hpp lex.yy.c
	$(CC) $(CFLAGS) $(SOURCES) -o asembler.out


clean:
	rm -f asembler.out  
	rm -f bison_gen.tab.cpp bison_gen.tab.hpp
	rm -f lex.yy.c
