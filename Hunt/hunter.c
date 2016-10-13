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
	//printf("%d\n", maxRail);

	//On the first turn, spread the 4 hunters out across the board
	if (round == 0){
		switch (player){
			case PLAYER_LORD_GODALMING :
			registerBestPlay("SR", "Saragoss(Spain)"); break;
			case PLAYER_DR_SEWARD :
			registerBestPlay("PA", "Paris(France)"); break;
			case PLAYER_VAN_HELSING :
			registerBestPlay("SZ", "Szeged(Yugoslavia)"); break;
			case PLAYER_MINA_HARKER :
			registerBestPlay("LI", "Leipzig(Germany)"); break;
		}
		return;
	}
	printf("%d", player);

	//See if Draculas position is known and set the most recent know location to be the target
	LocationID * dracTrail = malloc(TRAIL_SIZE*sizeof(LocationID));
	LocationID target = CITY_UNKNOWN;
	giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
	int i;
	printf("dracTrail = (");
	for(i = 0; dracTrail[i] != '\0'; i++){
		printf("%s,", idToAbbrev(dracTrail[i]));
		if (dracTrail[i] >= MIN_MAP_LOCATION && dracTrail[i] <= MAX_MAP_LOCATION && target == CITY_UNKNOWN){
			target = dracTrail[i];
		}
	}
	printf(")\n");

	printf("Target = %s\n", idToAbbrev(target));

	//Move to a random spot
	int numLocs;
	LocationID *locations = whereCanIgo(gameState, &numLocs, TRUE, maxRail, TRUE);
	printf("Where Can I go from %s with %d rail moves = (", idToAbbrev(whereIs(gameState, player)), maxRail);
	for(i = 0; locations[i] != '\0'; i++) {
		printf("%s,", idToAbbrev(locations[i]));
		if (locations[i] == target){
			registerBestPlay(idToAbbrev(target), message);
			return;
		}
	}
	printf(")\n");


	srand(MAGIC_NUMBER);
	LocationID selection = locations[rand() % numLocs];
	location = idToAbbrev(selection);

    registerBestPlay(location, message);
}


	//**I had a look and I dont think messages are gonna help us much**
	//**Ill leave it here in case we wanna add it back in**
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