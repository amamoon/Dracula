// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct hunterView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView Game;
    int round;
    int score;
    int player[NUM_PLAYERS];
    int health[NUM_PLAYERS];
    PlayerMessage messages[TRAIL_SIZE];
    LocationID trail[TRAIL_SIZE][NUM_PLAYERS];
    //int hello;
};
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->Game = newGameView(pastPlays, messages);
    hunterView->score = 366;
    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getRound(currentView->Game);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return (getCurrentPlayer(currentView->Game));
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getHealth(currentView->Game, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getLocation(currentView->Game, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    getHistory(currentView->Game, player, trail);
    
    /*int i = 0;
    for (i = 0; i < TRAIL_SIZE; i++) {
        trail[i] = currentView->trail[i][player];
	}*/
    
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return whereCanTheyGo(currentView, numLocations, whoAmI(currentView), road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
   //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
   return connectedLocations(currentView->Game, numLocations,
   		whereIs(currentView, player), player,giveMeTheRound(currentView),
   		road, rail, sea);
}
