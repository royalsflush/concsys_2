CC = gcc
CFLAGS = --ansi --pedantic -Wall -Wshadow
OUT = cond_vars

.PHONY: cond_vars all sample

all: sample cond_vars

sample:
	$(CC) $(CFLAGS) sample.c -o sample

cond_vars: cond_vars.c
	$(CC) $(CFLAGS) cond_vars.c -o cond_vars
