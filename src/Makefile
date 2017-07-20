#!/usr/bin/make -f

CFLAGS = -Wall -std=gnu99 -g -O2
LDFLAGS =

#LOG_DIR = /var/log/crashceptor
#ORIG_BIN_DIR = /var/lib/crashceptor

CPPDEFS = $(if $(LOG_DIR),-DLOG_DIR='"$(LOG_DIR)"') \
          $(if $(ORIG_BIN_DIR),-DORIG_BIN_DIR='"$(ORIG_BIN_DIR)"')

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
	$(CC) -c $(strip $(CFLAGS) $(CPPDEFS)) $(filter %.c,$^)

#-----------------------------------------------------------------------------
# vim:ft=make
