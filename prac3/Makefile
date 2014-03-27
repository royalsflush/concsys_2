CC = gcc
CFLAGS = --ansi --pedantic -Wall -Wshadow
SRC = printer.c printer.h sem.c semaphore.c semaphore.h util.h util.c
OUT = sem

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
