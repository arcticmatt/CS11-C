#
# CS 11: Makefile for C track, assignment 5.
#

CC     = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

all: 1dca_array 1dca_pointer

1dca_array: 1dca_array.o memcheck.o
	$(CC) 1dca_array.o memcheck.o -o 1dca_array

1dca_pointer: 1dca_pointer.o memcheck.o
	$(CC) 1dca_pointer.o memcheck.o -o 1dca_pointer

memcheck.o: memcheck.c memcheck.h
	$(CC) $(CFLAGS) -c memcheck.c

1dca_array.o: 1dca_array.c memcheck.h
	$(CC) $(CFLAGS) -c 1dca_array.c

1dca_pointer.o: 1dca_pointer.c memcheck.h
	$(CC) $(CFLAGS) -c 1dca_pointer.c

check:
	c_style_check 1dca_array.c 1dca_pointer.c

clean:
	rm -f *.o 1dca_array 1dca_pointer


