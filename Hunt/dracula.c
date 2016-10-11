// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
    //If its the first round
    //This prevents crashes due to passing an invalid location
    //(Which all players start at)
    if (whereIs(gameState, PLAYER_DRACULA) == UNKNOWN_LOCATION) {
        registerBestPlay("CD","Mwuhahahaha");
        return; //Escape function to avoid passing invalid parameters
    }
    
    //Get possible moves and store in two separate arrays
    //One for land moves, one for seas moves
    LocationID *roads, *seas;
    int *numRoads, *numSeas;
    numRoads = malloc(sizeof(int));
    numSeas = malloc(sizeof(int));
    roads = whereCanIgo(gameState, numRoads, TRUE, FALSE);
    seas = whereCanIgo(gameState, numSeas, FALSE, TRUE);
    
    //Select a random location to move to
    int i;
    char *location;
    srand(time(NULL));
    i = rand() % (*numRoads + 1);
    
    //Redundant line to make compiler happy about unused variables
    location = idToAbbrev(seas[i]);
    
    location = idToAbbrev(roads[i]);
    
    registerBestPlay(location, "Mwuhahahaha");
}
