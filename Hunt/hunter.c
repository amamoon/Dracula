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
	playerID player = whoAmI(gameState);
	int maxRail = ((int)player + giveMeTheRound(gameState)) % 4;

	//If I can see any of draculas recent locations, pop "H**" with ** being the location
	char * trail = malloc(8*TRAIL_SIZE*sizeof(char));
	giveMeTheTrail(gameState, player, trail);

	int i;
	for (i = 0; trail[i] != '\0'; i++){
		if (i%8 == 0){
			if (trail[i] == 'D' && trail[i+2] != '?'){
				message[0] = 'H';
				message[1] = trail[i+1];
				message[2] = trail[i+2];
				message[3] = '\0';
			}
		}
	}

	//Move to a random spot
	

    registerBestPlay(location, message);
}
