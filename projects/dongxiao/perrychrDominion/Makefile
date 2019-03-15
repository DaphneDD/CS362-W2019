CFLAGS= -Wall -fpic -coverage -lm -std=c99 -g

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


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
test_unittest1: unittest1
	./unittest1
	gcov -b dominion.c

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
test_unittest2: unittest2
	./unittest2
	gcov -b dominion.c

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
test_unittest3: unittest3
	./unittest3
	gcov -b dominion.c

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
test_unittest4: unittest4
	./unittest4
	gcov -b dominion.c

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
test_cardtest1: cardtest1
	./cardtest1
	gcov -b dominion.c

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
test_cardtest2: cardtest2
	./cardtest2
	gcov -b dominion.c

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
test_cardtest3: cardtest3
	./cardtest3
	gcov -b dominion.c

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)
test_cardtest4: cardtest4
	./cardtest4
	gcov -b dominion.c

unittestresults: 
	make test_cardtest1 1> unittestresults.out
	make test_cardtest2 1>> unittestresults.out
	cat dominion.c.gcov 1>> unittestresults.out
	
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
	time ./randomtestadventurer.out 1>randomtestresults.out 
	time ./randomtestcard1.out 1>>randomtestresults.out 
	gcov -b dominion.c
	cat dominion.c.gcov 1>>randomtestresults.out

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
