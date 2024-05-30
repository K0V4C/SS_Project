CC = g++
LEXER = flex
BISON = bison

PFLAGS = -d -t
CFLAGS = -std=c++17
ASM_FLAGS = -Ifl -Iinc/asembler/asm_actions -I/inc/common -I.
LINKER_FLAGS = -Iinc/linker -I/inc/common -I.

BISON_FILE = misc/bison_gen.ypp
FLEX_FILE = misc/flex_gen.lpp

ASM_SOURCES = $(wildcard *.cpp) $(wildcard *.c) $(wildcard src/asembler/*.cpp) $(wildcard src/common/*.cpp) $(wildcard src/asembler/asm_actions/*.cpp)
LINKER_SOURCES = $(wildcard src/linker/*.cpp) $(wildcard src/common/*.cpp)

bison_gen.tab.cpp bison_gen.tab.hpp: $(BISON_FILE)
	$(BISON) $(PFLAGS) $(BISON_FILE)

lex.yy.c: $(FLEX_FILE) bison_gen.tab.hpp
	$(LEXER) $(FLEX_FILE)

asembler_build: bison_gen.tab.cpp bison_gen.tab.hpp lex.yy.c
	$(CC) $(CFLAGS) $(ASM_FLAGS) $(ASM_SOURCES) -o "asembler"

linker_build:
	$(CC) $(CFLAGS) $(LINKER_FLAGS) $(LINKER_SOURCES) -o "linker"

all: linker_build asembler_build
	@echo "Project built!"




clean:
	rm -f linker.out
	rm -f asembler.out
	rm -f bison_gen.tab.cpp bison_gen.tab.hpp
	rm -f lex.yy.c
	rm -f *.o
