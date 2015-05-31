COMPILER = gcc
STANDARD = c99
FLAGS = -Wall

sstring.o :
	$(COMPILER) $(FLAGS) -std=$(STANDARD) -c sstring.c

