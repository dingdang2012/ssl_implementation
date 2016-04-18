CC=gcc
CFLAGS=-Wall -g
SUBDIR=base64 test
.PHONY: client clean $(SUBDIR)

all: finish

client: http.o client.o misc.o
	$(CC) $(CFLAGS) http.o client.o misc.o -o client

http.o: http.c
	$(CC) $(CFLAGS) -c http.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

misc.o: misc.c
	$(CC) $(CFLAGS) -c misc.c

base64:
	@echo ">>> Building base64 library"
	@make -C base64

test: base64
	@echo ">>> Building test case"
	@make -C test

tags:
	ctags -R

finish: client $(SUBDIR)

clean:
	rm -f client http.o client.o misc.o core.* tags
	for dir in $(SUBDIR); \
		do make -C $$dir clean || exit 1; \
	done;
