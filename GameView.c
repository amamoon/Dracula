// GameView.c ... GameView ADT implementation
//Edited James - 16-7 4pm

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT

typedef struct players * Players;
     
struct gameView {
    int turn;                       //The current turn starting at 1
    int round;                      //Current round, starting at 0, increments every 5 turns
    int score;                      //The hunters' score starting at 366
    Players player[NUM_PLAYERS];    //5 structs containing the players data
    PlayerMessage messages[TRAIL_SIZE]; //messages here or in player data ? - akeef
};

struct players {
   int health;
   char** trail[TRAIL_SIZE];  //Array of arrays containing the last 5 moves for dracula; initialised;
   //TODO Im thinking or turning this into a Queue, a friend showed me how he implemented one.
   
 };



//This function determines if a given character corresponds to the correct player
int nameEqChar(int player, char character){
    if (player == PLAYER_LORD_GODALMING && character == 'G')return 1;
    if (player == PLAYER_DR_SEWARD && character == 'S')return 1;
    if (player == PLAYER_VAN_HELSING && character == 'H')return 1;
    if (player == PLAYER_MINA_HARKER && character == 'M')return 1;
    if (player == PLAYER_DRACULA && character == 'D')return 1;
    return 0;
}

//Goes through past plays and finds any instance a score change has been triggered
int setScore( char * pastPlays){
    int score = 366, i;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if (i%8== 0){
            if (nameEqChar(PLAYER_DRACULA, pastPlays[i]))score--;
            if (!nameEqChar(PLAYER_DRACULA, pastPlays[i]) && pastPlays[i+1] == 'J' && pastPlays[i+2] == 'M')score-=6;
            //TODO ADD score-=13 if vampire matures
        }
    }
    return score;
}


// Goes through all pastplays and counts how many turns have passed
int setTurn(char * pastPlays){
    int i, turn = 0;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if (i % 8 ==0) turn++;
    }
    return turn +1;

}

//A function that isolates all of the strings in pastplays belonging to one character
void setTrail(GameView g, int player, char * pastPlays){
    int i, trailCount;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if(i%8==0 && nameEqChar(player, pastPlays[trailCount])){
            g->player[player]->trail[trailCount] = malloc(7);
            int j;
            for (j = 0; j < 7; j++) g->player[player]->trail[i][j] = pastPlays[i+j];
            trailCount++;
        }
    }


}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    
    //intialise game variables
    gameView->turn = setTurn(pastPlays);
    gameView->round = (gameView->turn -1)/5;
    gameView->score = setScore(pastPlays); 
    
    //intitialise hunters
    int i;
    for (i = 0; i < PLAYER_DRACULA; i++){
        gameView->player[i] = malloc(sizeof(struct players)); 
        gameView->player[i]->health = GAME_START_HUNTER_LIFE_POINTS;
        gameView->player[i]->trail = malloc(7*GAME_START_SCORE);
        setTrail(gameView, i, pastPlays);
    }
    
    //initialise dracula
    gameView->player[PLAYER_DRACULA] = malloc(sizeof(struct players)); 
    gameView->player[PLAYER_DRACULA]->health = GAME_START_BLOOD_POINTS;
    gameView->player[PLAYER_DRACULA]->trail = malloc(7*GAME_START_SCORE);
    setTrail(gameView, PLAYER_DRACULA, pastPlays);
    
    //add the trail 
    
    return gameView;
}
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    
    int i;
    for (i = 0; i <= PLAYER_DRACULA; i++){
        int j;
        for(j=0; toBeDeleted->player[i]->trail[j] != NULL; j++){
            free(toBeDeleted->player[i]->trail[j]);
        }
        free(toBeDeleted->player[PLAYER_DRACULA]->trail);
        free(toBeDeleted->player[i]);
    }
    
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    return currentView->round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    return ((currentView->turn - 1) % 5);
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->player[player]->health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    int i;
    for (i = 0; currentView->player[player]->trail[i] != NULL; i++){}
    i--;
    char * location = malloc(2);
    location[0] = currentView->player[player]->trail[i][1];
    location[1] = currentView->player[player]->trail[i][2];

    return abbrevToID(location);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
// just filler for rn could use more detail and debugging
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int i ,n;
    for(i = 0, n = 0; i < TRAIL_SIZE; i++, n++)
          trail[i] = currentView->location[player][n];
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}


//PRINT FUNCTIONS FOR DEBUGGING
//add a ifdef?
#ifndef PRINT
void printIndex (char *indexName, LocationID *index, int size){
    printf("*********\n%s:\n", indexName);
    int i;
    for (i = 0; i < size+1; i++) {
        if (index[i] == CONNECTED) printf("-- %d --", i); 
    } 
    printf("\n*********\n");
}

void printList (char *listName, LocationID *list, int size){
    printf("*********\n%s:\n", listName);
    int i;
    for (i = 0; i < size; i++) {
        if (list[i] == NOWHERE) break;
        printf("%d: %d   ", i, list[i]);   
        if (((i % 9) == 0) && i != 0) printf("\n");
    } 
    printf("\n*********\n");
}
 
 
void printLinkIndex (char *name, Link a, int size){
    printf("************\n%s\n************\n", name);
    int i;
    for (i = 0; i < size; i++) {
        if (a[i].loc != NOWHERE) {
            printf("%s[%d]:    loc: %d    type: %d    dist: %f\n", name, 
                    i, a[i].loc, a[i].type, a[i].dist);    
        }
    }
    printf("fin\n************\n");
}

void printLinkList (char *name, Link a, int size) {
    printf("************\n%s\n************\n", name);
    int i;
    for (i = 0; i < size && a[i].loc != NOWHERE; i++) {
        printf("%s[%d]:    loc: %d    type: %d    dist: %f\n", name, 
               i, a[i].loc, a[i].type, a[i].dist);    
    }
    printf("fin\n************\n");

}

void printLinkLoc (char *name, Link a, int size) {
    printf("************\n%s\n************\n%s:", name, name);
    int i;
    for (i = 0; i < size && a[i].loc != NOWHERE; i++) {
        printf(" %d->%d ", i, a[i].loc); 
        if (i % 9 == 0) printf("\n");   
    }
    printf("\nfin\n************\n");
}
#endif
