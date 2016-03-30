CC=gcc
CFLAGS=-Wall -g
.PHONY: client clean
client: http.o client.o misc.o
	gcc http.o client.o misc.o -o client

clean:
	rm -f client http.o client.o misc.o core.*
