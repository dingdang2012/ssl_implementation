.PHONY: all clean

OBJ = libbase64.a libbase64.so base64.o

all: $(OBJ)

libbase64.a: base64.o
	ar cr libbase64.a base64.o

libbase64.so: base64.o
	gcc -shared -fPIC -o libbase64.so base64.o

base64.o: base64.c
	gcc -Wall -g -c -fPIC base64.c

clean:
	rm -f *.o *.a *.so
