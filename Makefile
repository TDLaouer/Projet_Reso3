BIN = executable
OBJECTS = main.o semantic.o
CC = gcc
CFLAGS = -Wall -pedantic

export LD_LIBRARY_PATH =.

all: 	$(OBJECTS)
	gcc $(OBJECTS) -o $(BIN) -L. -lrequest -lparser

main.o: main.c
	gcc -g -l./api -c main.c

semantic.o: semantic.c semantic.h
	$(CC) -c semantic.c $(CFLAGS) -o semantic.o
