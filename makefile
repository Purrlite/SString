COMPILER = gcc
STANDARD = c11
FLAGS = -Wall

sstring.o :
	$(COMPILER) $(FLAGS) -std=$(STANDARD) -c sstring.c

