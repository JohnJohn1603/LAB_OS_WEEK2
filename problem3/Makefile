CC = gcc
CFLAGS = -Wall -lpthread
TARGETS = process1 process2

all: $(TARGETS)

process1: process1.c
	$(CC) $(CFLAGS) -o process1 process1.c

process2: process2.c
	$(CC) $(CFLAGS) -o process2 process2.c

clean:
	rm -f $(TARGETS)
