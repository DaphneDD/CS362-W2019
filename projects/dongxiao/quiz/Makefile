CFLAGS= -g -Wall -fpic -coverage -lm -std=c99

testme.o: testme.c
	gcc -c testme.c $(CFLAGS)

testme: testme.o
	gcc -o testme testme.o $(CFLAGS)

all: testme

clean: 
	rm -f *.o 
