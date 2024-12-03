+.PHONY: all
.SUFFIXES: # remove default suffix rules

CC = g++ # fortran compiler
CFLAGS = -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -ggdb
CFLAGS_RELEASE = -O3 -DNDEBUG

MAIN = Day1.cpp
EXECUTABLE = aoc.exe

# should cause clean to be system-agnostic
RM = rm -f
ifdef OS
RM = del
endif

all: debug

debug:
	@$(CC) $(CFLAGS) $(MAIN) -o $(EXECUTABLE)

release: 
	@$(CC) $(CFLAGS_RELEASE) $(MAIN) -o $(EXECUTABLE)

clean: 
	$(RM) *.o *.exe *.mod $(TARGET)

help:
	@echo Valid Targets:
	@echo  all: creates aoc.exe in debug mode
	@echo  debug: creates aoc.exe in debug mode
	@echo  release: creates aoc.exe in release mode
	@echo  clean: uses $(RM) [Env. Var. OS is $(OS)]
	@echo    then: removes .o .mod .exe files, and TARGET=file
	@echo  help: prints list of valid targets


