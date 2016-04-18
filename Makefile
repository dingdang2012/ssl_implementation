CC=gcc
CFLAGS=-Wall -g
SUBDIR=base64 test
.PHONY: client clean $(SUBDIR)

all: finish

base64:
	@echo ">>> Building base64 library"
	@make -C base64

http.o: http.c base64
	$(CC) $(CFLAGS) -c http.c -lbase64

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

misc.o: misc.c
	$(CC) $(CFLAGS) -c misc.c

client: http.o client.o misc.o base64
	$(CC) $(CFLAGS) http.o client.o misc.o -o client -L./base64 -lbase64

test: base64
	@echo ">>> Building test case"
	@make -C test

tags:
	ctags -R

finish: $(SUBDIR) client

clean:
	rm -f client http.o client.o misc.o core.* tags
	for dir in $(SUBDIR); \
		do make -C $$dir clean || exit 1; \
	done;
