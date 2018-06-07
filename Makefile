BIN = executable
CC = gcc
OBJECTS = main.o semantic.o socket.o
CFLAGS = -Wall -pedantic -std=c99

export LD_LIBRARY_PATH=.

all: $(OBJECTS)
	gcc $(OBJECTS) -o $(BIN) -L. -lrequest -lparser

main.o: main.c
	gcc -g -l./api -c main.c

semantic.o: semantic.c semantic.h
	$(CC) -c semantic.c $(CFLAGS) -o semantic.o

socket.o: socket.c socket.h
	$(CC) -c socket.c $(CFLAGS) -o socket.o

clean:
	rm *.o
