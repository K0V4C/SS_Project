CC = g++
LEXER = flex
BISON = bison

PFLAGS = -d -t
CFLAGS = -std=c++17
ASM_FLAGS = -Ifl -Iinc/asembler/asm_actions -I/inc/common -I.
LINKER_FLAGS = -Iinc/linker -I/inc/common -I.
EMULATOR_FLAGS = -Iinc/emulator -I. -pthread

BISON_FILE = misc/bison_gen.ypp
FLEX_FILE = misc/flex_gen.lpp

ASM_SOURCES = $(wildcard *.cpp) $(wildcard *.c) $(wildcard src/asembler/*.cpp) $(wildcard src/common/*.cpp) $(wildcard src/asembler/asm_actions/*.cpp)
LINKER_SOURCES = $(wildcard src/linker/*.cpp) $(wildcard src/common/*.cpp)
EMULATOR_SOURCES = $(wildcard src/emulator/*.cpp)

bison_gen.tab.cpp bison_gen.tab.hpp: $(BISON_FILE)
	$(BISON) $(PFLAGS) $(BISON_FILE)

lex.yy.c: $(FLEX_FILE) bison_gen.tab.hpp
	$(LEXER) $(FLEX_FILE)

asembler_build: bison_gen.tab.cpp bison_gen.tab.hpp lex.yy.c
	$(CC) $(CFLAGS) $(ASM_FLAGS) $(ASM_SOURCES) -o "assembler"

linker_build:
	$(CC) $(CFLAGS) $(LINKER_FLAGS) $(LINKER_SOURCES) -o "linker"

emulator_build:
	$(CC) $(CFLAGS) $(EMULATOR_FLAGS) $(EMULATOR_SOURCES) -o "emulator"

all: emulator_build linker_build asembler_build
	@echo "Project built!"

asembler_debug: bison_gen.tab.cpp bison_gen.tab.hpp lex.yy.c
	$(CC) $(CFLAGS) -g $(ASM_FLAGS) $(ASM_SOURCES) -o "assembler" -DDEBUG

linker_debug:
	$(CC) $(CFLAGS) -g $(LINKER_FLAGS) $(LINKER_SOURCES) -o "linker" -DDEBUG

emulator_debug:
	$(CC) $(CFLAGS) -g $(EMULATOR_FLAGS) $(EMULATOR_SOURCES) -o "emulator" -DDEBUG

debug: emulator_debug linker_debug asembler_debug
	@echo "Debug buildt"

clean:
	rm -f linker
	rm -f assembler
	rm -f emulator
	rm -f bison_gen.tab.cpp bison_gen.tab.hpp
	rm -f lex.yy.c
	rm -f *.o
	rm -f *.hex
	rm -f *.txt
