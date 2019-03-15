// test implementation of smithy.

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NOISY_TEST 1

int debug = 0; //a global variable to indicate whether the debugging message will be printed out
int nGenerate = 0;

/********************************************************************************************
 * Function: assertTrue
 * Parameters: testResult: an int; expectedResult: an int; info: a string
 * Return Value: If testResult is the same as expectedResult, then it returns 1. Otherwise 0.
 * Description: The test wil print out info only if the global variable debug is nonzero. If 
 * 		testResult is the same as expectedResult, then the "TEST SUCCESSFULLY 
 * 		COMPLETED" will be displayed on stdout. Otherwise, "TEST FAILED" will be
 * 		printed out. The message is followed by the testing conditions stated in info.
 * *******************************************************************************************/
int assertTrue(int testResult, int expectedResult, char* info)
{
	if (testResult == expectedResult)
	{
		if (debug)
		{
			printf("TEST SUCCESSFULLY COMPLETED: "); fflush(stdout);
			printf(info);
		}
		return 1;
	}
	else
	{
		if (debug)
		{
			printf("TEST FAILED: "); fflush(stdout);
			printf(info);
		}
		return 0;
	}
}

/********************************************************************************************
 * Function: checkSmithy
 * Parameters: player: an int, the current player; post: a pointer to a struct gameState
 * Return Value: If the implementation of smithy does what as expected, then it returns 1. 0 otherwise.
 * Precondition: The post must point to a valid struct gameState for smithy card. That is, 
 * 		 there must be at least three cards in the deck and discard pile.
 * Postcondition: the content of the *post is altered.
 * Description: If the global variable debug is nonzero, then the test conditions will be printed
 * 		to stdout. This function test whether three cards are added to the hand,
 *		and whether the total number of cards in the player's possion remains unchanged.
 *		If these two criteria are met, then the function returns 1. 0 otherwise.
 * *******************************************************************************************/
int checkSmithy(int player, struct gameState *post, int handPos) {
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));

	int result = 1; //check whether the expected values are the same as test values
	int comp;
	int bonus = 0;
	cardEffect(smithy,0,0,0,post,0,&bonus); //call the smithyAction through cardEffect

	//check whether two more cards are added to the hand
	comp = assertTrue(pre.handCount[player]+2, post->handCount[player], "2 cards should be added to hand\n");
	result = result && comp;
	if (debug)
	{
		printf("%d cards were added to hand\n", post->handCount[player] - pre.handCount[player]);
		fflush(stdout);
	}
	
	//check whether the total number of cards the player has remains unchanged
	int preTotal = pre.handCount[player] + pre.deckCount[player] + pre.discardCount[player] + pre.playedCardCount;
	int postTotal = post->handCount[player] + post->deckCount[player] + post->discardCount[player] + post->playedCardCount;
	comp = assertTrue(preTotal, postTotal, "Player's total card count should remain unchanged\n");
	result = result && comp;
	if (debug)
	{
		printf("Expected total card count: %d; Actual total card count: %d\n", preTotal, postTotal);
		fflush(stdout);
	}	
	
	//check whether the smithy card is added to the played card pile.
	comp = assertTrue(post->playedCardCount, pre.playedCardCount + 1, "1 card should be added to the played pile\n");
	result = result && comp;
	comp = assertTrue(post->playedCards[pre.playedCardCount], smithy, "The new played card should be smithy\n");
	result = result && comp;
	
	return result;
}


/********************************************************************************************
 * Function: generateSmithyTestCase
 * Parameters: game: a pointer to a struct gameState
 * Return Value: the hand position of the smithy card
 * Precondition: game must point to a struct gameState whose memory has been allocated.
 * Postcondition: the *game has been filled with random data that is suitable for test of
 * 		  smithyAction().
 * Description: This function fills *game with random data. It makes sure that the deckCount,
 * 		handCount, discardCount, and whoseTurn are all valid numbers. It also makes 
 * 		sure that the player has at least three cards to draw from
 * *******************************************************************************************/
int generateSmithyTestCase(struct gameState *game) 
{	
	int i, j, player, handPos;
	while(1)
	{
		nGenerate++;
		for (i = 0; i < sizeof(struct gameState); i++) 
			((char*)game)[i] = floor(Random() * 256);
		player = floor(Random() * 4);
		game->whoseTurn = player;
		game->deckCount[player] = floor(Random() * MAX_DECK);
		game->discardCount[player] = floor(Random() * MAX_DECK);
		game->handCount[player] = floor(Random() * (MAX_HAND - 1)) + 1;
		game->playedCardCount = floor(Random() * (MAX_DECK - 1));
		// make sure the player has at least three cards to draw from
		if (game->deckCount[player] + game->discardCount[player] < 3)
			continue;

		// select random cards for the deck
		for (j=0; j < game->deckCount[player]; j++)
			game->deck[player][j]= (floor(Random() * (treasure_map + 1)));
		// select random cards for the discard pile
		for (j=0; j < game->discardCount[player]; j++)
			game->discard[player][j] = (floor(Random() * (treasure_map + 1)));
		// select handPos
		handPos = floor(Random() * game->handCount[player]);
		game->hand[player][handPos] = smithy;
		// the game is suitable to test smithyAction(). break out of the loop.
		break;
	}
	return handPos;
}

int main () {
//	int i, n, r, p, deckCount, discardCount, handCount;
	int seed = 1542;
	int nTest = 0, nDebug = 0, nSuccess = 0, nFailure = 0;
	int i, pass, handPos;
	const int MAXTEST = 10000, MAXDEBUG = 5;

	struct gameState G;

	printf ("\n\n*************************  Random Testing - Smithy  ***************************\n");
	fflush(stdout);

	SelectStream(2);
	PutSeed(seed);

	for (i = 0; i < MAXTEST; i++) 
	{
		nTest++;
		handPos = generateSmithyTestCase(&G);	
		if (debug)
		{
			printf("*************** TEST Case #%d **************\n", nDebug);
			fflush(stdout);
		}
		pass = checkSmithy(G.whoseTurn, &G, handPos);
	
		if (pass)
			nSuccess++;
		else
		{
			nFailure++;
			if (nDebug == 0) //if this is the first failure, then turn on debug
				debug = 1;
		}
		if (debug)
		{
			nDebug++;
			if (nDebug > MAXDEBUG) // if MAXDEBUG messages have been printed out, then turn off debug
				debug = 0;
		}
	}

	printf ("\n\n****** SUMMARY: Generated %d cases, %d passed, %d failed ******\n\n", nTest, nSuccess, nFailure);

	return 0;
}
