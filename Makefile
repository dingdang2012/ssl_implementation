CC=gcc
CFLAGS=-Wall
.PHONY: client clean
client: http.o client.o
	gcc http.o client.o -o client

clean:
	rm -f client http.o client.o
