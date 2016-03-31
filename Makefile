CC=gcc
CFLAGS=-Wall -g
.PHONY: client clean

client: http.o client.o misc.o
	$(CC) $(CFLAGS) http.o client.o misc.o -o client

http.o: http.c
	$(CC) $(CFLAGS) -c http.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

misc.o: misc.c
	$(CC) $(CFLAGS) -c misc.c

clean:
	rm -f client http.o client.o misc.o core.*
