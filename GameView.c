// GameView.c ... GameView ADT implementation
//Edited James - 16-7 4pm

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT

#define TRAIL_SECTION_LENGTH 8

typedef struct players * Players;
     
struct gameView {
    int turn;                       //The current turn starting at 1
    int round;                      //Current round, starting at 0, increments every 5 turns
    int score;                      //The hunters' score starting at 366
    Players player[NUM_PLAYERS];    //5 structs containing the players data
    PlayerMessage messages[MESSAGE_SIZE]; //messages here or in player data ? - akeef
};

struct players {
   int health;
   char** trail;  //Array of arrays containing the last 5 moves for dracula; initialised;
   //TODO Im thinking or turning this into a Queue, a friend showed me how he implemented one.
   
 };



//This function determines if a given character corresponds to the correct player
static int nameEqChar(int player, char character){
    if (player == PLAYER_LORD_GODALMING && character == 'G')return 1;
    if (player == PLAYER_DR_SEWARD && character == 'S')return 1;
    if (player == PLAYER_VAN_HELSING && character == 'H')return 1;
    if (player == PLAYER_MINA_HARKER && character == 'M')return 1;
    if (player == PLAYER_DRACULA && character == 'D')return 1;
    return 0;
}

//Goes through past plays and finds any instance a score change has been triggered
int setScore( char * pastPlays){
    int score = GAME_START_SCORE, i;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if (i%8== 0){
            if (nameEqChar(PLAYER_DRACULA, pastPlays[i])) score-= SCORE_LOSS_DRACULA_TURN;
            if (!nameEqChar(PLAYER_DRACULA, pastPlays[i]) && pastPlays[i+1] == 'J' && pastPlays[i+2] == 'M')score-=SCORE_LOSS_HUNTER_HOSPITAL;
            if (!nameEqChar(PLAYER_DRACULA, pastPlays[i]) && pastPlays[i+5] == 'V') score-= SCORE_LOSS_VAMPIRE_MATURES;
        }
    }
    return score;
}


// Goes through all pastplays and counts how many turns have passed
static int setTurn(char * pastPlays){
    int i, turn = 0;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if (i % TRAIL_SECTION_LENGTH ==0) turn++;
    }
    return turn +1;

}

//A function that isolates all of the strings in pastplays belonging to one character
static void setTrail(GameView g, int player, char * pastPlays){
    int i, trailCount;
    for (i = 0; pastPlays[i] != '\0'; i++){
        if(i%TRAIL_SECTION_LENGTH==0 && nameEqChar(player, pastPlays[trailCount])){
            g->player[player]->trail[trailCount] = malloc(7);
            int j;
            for (j = 0; j < 7; j++) g->player[player]->trail[i][j] = pastPlays[i+j];
            trailCount++;
        }
    }
}

//Determines if two chars are for a sea/ocean
static int trueSea(char first, char second){
    if (first == 'A' && second == 'S') return 1; //Adreatic Sea
    if (first == 'A' && second == 'O') return 1; //Atlantic Ocean
    if (first == 'B' && second == 'B') return 1; //Bay of Biscay
    if (first == 'B' && second == 'S') return 1; //Black Sea
    if (first == 'E' && second == 'C') return 1; //English Channel
    if (first == 'I' && second == 'O') return 1; //Ionian Sea
    if (first == 'I' && second == 'R') return 1; //Irish Sea
    if (first == 'M' && second == 'S') return 1; //Mediterranean Sea
    if (first == 'N' && second == 'S') return 1; //North Sea
    if (first == 'T' && second == 'S') return 1; //Tyhrennian Sea
    return 0;
}

//A function that determines the score for each of the characters
static int setHealth(GameView g, int player){
    char ** playerTrail = g->player[player]->trail;
    int health = 0;
    if (player >= PLAYER_LORD_GODALMING && player <= PLAYER_MINA_HARKER){
        health = GAME_START_HUNTER_LIFE_POINTS;

        int i;
        for (i = 0; playerTrail[i] != NULL; i++){
            if (playerTrail[i][3] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;//trap
            if (playerTrail[i][4] == 'D') health -= LIFE_LOSS_DRACULA_ENCOUNTER;//dracula
            if (i != 0 && playerTrail[i][1] == playerTrail[i-1][1] && playerTrail[i][2] == playerTrail[i-1][2]) health += LIFE_GAIN_REST;//rest

            if (health <= 0) health = 9;
        }

    } else {
        health = GAME_START_BLOOD_POINTS;

        int j;
        for (j = 0; playerTrail[j] != NULL; j++){   
            //hunter
            int k;
            for(k = 0; k < PLAYER_DRACULA; k++){
                if (g->player[k]->trail[j][4] == 'D') health -= LIFE_LOSS_HUNTER_ENCOUNTER;
            }

            if (trueSea(playerTrail[j][1], playerTrail[j][2])) health -= LIFE_LOSS_SEA;//sea TODO work out if C?
            if (playerTrail[j][1] == 'C' && playerTrail[j][2] == 'D') health += LIFE_GAIN_CASTLE_DRACULA;//castle
        }
    }


    return health;
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
        gameView->player[i]->trail = malloc(7*GAME_START_SCORE);
        setTrail(gameView, i, pastPlays);
        gameView->player[i]->health = setHealth(gameView, i);
    }
    
    //initialise dracula
    gameView->player[PLAYER_DRACULA] = malloc(sizeof(struct players)); 
    gameView->player[PLAYER_DRACULA]->trail = malloc(7*GAME_START_SCORE);
    setTrail(gameView, PLAYER_DRACULA, pastPlays);
    gameView->player[PLAYER_DRACULA]->health = setHealth(gameView, PLAYER_DRACULA);

    //TODO gameView->messages = messages;
    
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
// Slighlty adjusted this function -J
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int i ,n;
    for(i = 0; i < TRAIL_SIZE; i++){
          trail[i] = currentView->player[player]->trail[n][i % 8];
          if (i % 8 == 7) n++;
      }

}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea){
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

/*Awesome idea, but ill comment out now for compilation
//PRINT FUNCTIONS FOR DEBUGGING
//add a ifdef?
#ifndef PRINT

#include <stdio.h>
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
*/
