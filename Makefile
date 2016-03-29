CC=gcc
CFLAGS=-Wall
.PHONY: client clean
client: http.o
	gcc http.o -o client

clean:
	rm -f client http.o
