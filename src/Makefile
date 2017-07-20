#!/usr/bin/make -f

CFLAGS = -std=gnu99 -g -O2
LDFLAGS =

CC = gcc
LD = gcc

#-----------------------------------------------------------------------------

.PHONY: all
all: crashceptor

.PHONY: clean
clean:
	rm -f crashceptor *.o

crashceptor: crashceptor.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $(filter %.c,$^)

#-----------------------------------------------------------------------------
# vim:ft=make
