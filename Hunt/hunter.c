// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

#define MAGIC_NUMBER 1589

void decideHunterMove(HunterView gameState)
{
    // TODO ...
    // Replace the line below by something better
	char * location = malloc(2*sizeof(char));
	char * message = malloc(MESSAGE_SIZE*sizeof(char));
	PlayerID player = whoAmI(gameState);
	int round =giveMeTheRound(gameState);
	int maxRail = ((int)player + round) % 4;

	//If I can see any of draculas recent locations, pop "H**" with ** being the location
	/*LocationID * trail = malloc(TRAIL_SIZE*sizeof(LocationID));
	giveMeTheTrail(gameState, player, trail);

	int i;
	for (i = 0; trail[i] != '\0'; i++){
		if (trail[i] != -1){
			message[0] = 'H';
			message[1] = (char)(trail[i]/10 +'0');
			message[2] = (char)(trail[i]%10 +'0');
			message[3] = '\0';
		}
	}*/

	if (round == 0){
		registerBestPlay("GE", message);
		return;
	}

	//Move to a random spot
	int numLocs;
	LocationID *locations = whereCanIgo(gameState, &numLocs, TRUE, maxRail, TRUE);


	srand(MAGIC_NUMBER);

	LocationID selection = locations[rand() % numLocs];
	location = idToAbbrev(selection);

    registerBestPlay(location, message);
}
