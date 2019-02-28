CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

randomtestadventurer.o: randomtestadventurer.c dominion.h dominion_helpers.h rngs.h
	gcc -c randomtestadventurer.c -g $(CFLAGS)
randomtestadventurer.out: randomtestadventurer.o dominion.o rngs.o
	gcc -o randomtestadventurer.out -g randomtestadventurer.o dominion.o rngs.o $(CFLAGS)
randomtestcard1.o: randomtestcard1.c dominion.h dominion_helpers.h rngs.h
	gcc -c randomtestcard1.c -g $(CFLAGS)
randomtestcard1.out: randomtestcard1.o dominion.o rngs.o
	gcc -o randomtestcard1.out -g randomtestcard1.o dominion.o rngs.o $(CFLAGS)
randomtestcard2.o: randomtestcard2.c dominion.h dominion_helpers.h rngs.h
	gcc -c randomtestcard2.c -g $(CFLAGS)
randomtestcard2.out: randomtestcard2.o dominion.o rngs.o
	gcc -o randomtestcard2.out -g randomtestcard2.o dominion.o rngs.o $(CFLAGS)
randomTestAll: randomtestadventurer.out randomtestcard1.out randomtestcard2.out
	make randomtestadventurer.out
	make randomtestcard1.out
	make randomtestcard2.out
	time ./randomtestadventurer.out 1>randomtestresults.out 
	time ./randomtestcard1.out 1>>randomtestresults.out 
	time ./randomtestcard2.out 1>>randomtestresults.out
	gcov -b dominion.c
	cat dominion.c.gcov 1>>randomtestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe 
