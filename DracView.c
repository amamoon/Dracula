// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct dracView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView Game; //pls no
    int health[NUM_PLAYERS]; //been asking around and apparently this is better as an array? figured it out
    int nTrap[NUM_MAP_LOCATIONS];
    int score;
    LocationID trail[TRAIL_SIZE];
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[]){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->score = 366;
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted){
    //COMPLETE THIS IMPLEMENTATION
    assert(toBeDeleted != NULL);
    disposeGameView(toBeDeleted->Game);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getRound(currentView->Game); //smart
}

// Get the current score
int giveMeTheScore(DracView currentView){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->health[player]; // i see why its gotta be an array
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getLocation(currentView->Game, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    //go to 'where'
    //check data
    //return an array ?
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE]){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    // call connected connections
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    //call connected locations
    return NULL;
}
