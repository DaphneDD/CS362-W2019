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
 * Function: checkAdventurer
 * Parameters: player: an int, the current player; post: a pointer to a struct gameState
 * Return Value: If the adventurerAction does what as expected, then it returns 1. 0 otherwise.
 * Precondition: The post must point to a valid struct gameState for adventurer card. That is, 
 * 		 there must be at least two treasure card in the deck and discard pile.
 * Postcondition: the contant of the *post is altered.
 * Description: If the global variable debug is nonzero, then the test conditions will be printed
 * 		to stdout. This function test whether two treasure cards are added to the hand,
 *		and whether the total number of cards in the player's possion remains unchanged.
 *		If these two criteria are met, then the function returns 1. 0 otherwise.
 * *******************************************************************************************/
int checkAdventurer(int player, struct gameState *post) {
	struct gameState pre;
	memcpy (&pre, post, sizeof(struct gameState));

	int result = 1; //check whether the expected values are the same as test values
	int comp;
	int bonus = 0;
	cardEffect(adventurer,0,0,0,post,0,&bonus); //call the adventurerAction

	//check whether two more cards are added to the hand
	comp = assertTrue(pre.handCount[player]+2, post->handCount[player], "2 cards should be added to hand\n");
	result = result && comp;
	if (debug)
	{
		printf("%d cards were added to hand\n", post->handCount[player] - pre.handCount[player]);
		fflush(stdout);
	}
	//check whether the total number of cards the player has remains unchanged
	int preTotal = pre.handCount[player] + pre.deckCount[player] + pre.discardCount[player];
	int postTotal = post->handCount[player] + post->deckCount[player] + post->discardCount[player];
	comp = assertTrue(preTotal, postTotal, "Player's total card count should remain unchanged\n");
	result = result && comp;
	if (debug)
	{
		printf("Expected total card count: %d; Actual total card count: %d\n", preTotal, postTotal);
		fflush(stdout);
	}
	
	//check whether the two cards added to the hand are treasure cards
	int count = post->handCount[player];
	int card1 = post->hand[player][count-2];
	int card2 = post->hand[player][count-1];
	comp = assertTrue(card1 >= copper && card1 <= gold, 1, "first card added should be a treasure card\n");
	result = result && comp;
	if (debug)
	{
		printf("First card added to hand is %d\n", card1);
		fflush(stdout);
	}
	comp = assertTrue(card2 >= copper && card2 <= gold, 1, "second card added should be a treasure card\n");
	result = result && comp;
	if (debug)
	{
		printf("Second card added to hand is %d\n", card2);
		fflush(stdout);
	}
	
	return result;
}


/********************************************************************************************
 * Function: generateAdventurerTestCase
 * Parameters: game: a pointer to a struct gameState
 * Precondition: game must point to a struct gameState whose memory has been allocated.
 * Postcondition: the *game has been filled with random data that is suitable for test of
 * 		  adventurerAction().
 * Description: This function fills *game with random data. It makes sure that the deckCount,
 * 		handCount, discardCount, and whoseTurn are all valid numbers. It also makes 
 * 		sure that the player has at least two treasure cards to draw from
 * *******************************************************************************************/
void generateAdventurerTestCase(struct gameState *game) 
{	
	int i, j, player, nTreasure, tempCard;
	while(1)
	{
		nGenerate++;

		for (i = 0; i < sizeof(struct gameState); i++) 
			((char*)game)[i] = floor(Random() * 256);
		player = floor(Random() * 4);
		game->whoseTurn = player;
		game->deckCount[player] = floor(Random() * MAX_DECK);
		game->discardCount[player] = floor(Random() * MAX_DECK);
		game->handCount[player] = floor(Random() * MAX_HAND);
		// make sure the player has at least two cards to draw from
		if (game->deckCount[player] + game->discardCount[player] < 2)
			continue;

		nTreasure = 0;
		// select random cards for the deck
		for (j=0; j < game->deckCount[player]; j++)
		{
			tempCard = (floor(Random() * (treasure_map + 1)));
			game->deck[player][j] = tempCard;
			if (tempCard >= copper && tempCard <= gold)
				nTreasure++;

		}
		// select random cards for the discard pile
		for (j=0; j < game->discardCount[player]; j++)
		{
			tempCard = (floor(Random() * (treasure_map + 1)));
			game->discard[player][j] = tempCard;
			if (tempCard >= copper && tempCard <= gold)
				nTreasure++;
		}
		// make sure the player has at least two treasure cards to draw from
		if (nTreasure < 2)
			continue;
		// if the game is suitable to test adventurerAction(), then break out of the loop.
		break;
	}
}

int main () {
//	int i, n, r, p, deckCount, discardCount, handCount;
	int seed = 1542;
	int nTest = 0, nDebug = 0, nSuccess = 0, nFailure = 0;
	int i, pass;
	const int MAXTEST = 10000, MAXDEBUG = 5;
//	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

	struct gameState G;

	printf ("\n\n*************************  Random Testing - Acventurer  ***************************\n");
	fflush(stdout);

	SelectStream(2);
	PutSeed(seed);

	for (i = 0; i < MAXTEST; i++) 
	{
		nTest++;
		generateAdventurerTestCase(&G);	
		if (debug)
		{
			printf("*************** TEST Case #%d **************\n", nDebug);
			fflush(stdout);
		}
		pass = checkAdventurer(G.whoseTurn, &G);
	
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

	printf ("\n\n******SUMMARY: Generated %d cases, %d passed, %d failed******\n\n", nTest, nSuccess, nFailure);

	return 0;
}
