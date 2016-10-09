// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

void decideHunterMove(HunterView gameState)
{
    // TODO ...
    // Replace the line below by something better
	char * location = malloc(2*sizeof(char));
	char * message = malloc(MESSAGE_SIZE*sizeof(char));
	PlayerID player = whoAmI(gameState);
	int maxRail = ((int)player + giveMeTheRound(gameState)) % 4;

	//If I can see any of draculas recent locations, pop "H**" with ** being the location
	LocationID * trail = malloc(TRAIL_SIZE*sizeof(LocationID));
	giveMeTheTrail(gameState, player, trail);

	int i;
	for (i = 0; trail[i] != '\0'; i++){
		if (trail[i] != -1){
			message[0] = 'H';
			message[1] = (char)(trail[i]/10 +'0');
			message[2] = (char)(trail[i]%10 +'0');
			message[3] = '\0';
		}
	}

	//Move to a random spot
	int numLocs;
	LocationID *locations = whereCanIgo(gameState, &numLocs, TRUE, maxRail, TRUE);

	srand(69);

	LocationID selection = locations[rand() % numLocs];
	location = idToName(selection);

    registerBestPlay(location, message);
}
