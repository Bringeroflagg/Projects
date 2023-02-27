#
# "makefile" for the penultimate implementation of CONCORD (concord3.c)
# Summer 2021, assignment #3.
#

CC=gcc

# The line with -DDEBUG can be used for development. When
# building your code for evaluation, however, the line *without*
# the -DDEBUG will be used.
#
# The -D_GNU_SOURCE flag makes available gcc extensions to the
# C-language standard (for functions such as `getline()` and
# `strdup()`).
#

CFLAGS=-c -Wall -g -DDEBUG -D_GNU_SOURCE -std=c99


all: concord3

concord3: concord3.o seng265-list.o emalloc.o
	$(CC) concord3.o seng265-list.o emalloc.o -o concord3

concord3.o: concord3.c seng265-list.h emalloc.h
	$(CC) $(CFLAGS) concord3.c

seng265-list.o: seng265-list.c seng265-list.h emalloc.h
	$(CC) $(CFLAGS) seng265-list.c

emalloc.o: emalloc.c emalloc.h
	$(CC) $(CFLAGS) emalloc.c

clean:
	rm -rf *.o concord3 
