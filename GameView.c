// GameView.c ... GameView ADT implementation
//Edited James - 16-7 4pm

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

#define TRAIL_SECTION_LENGTH 8

typedef struct players * Players;
     
struct gameView {
    Map gameMap;
    int turn;                       //The current turn starting at 1
    int round;                      //Current round, starting at 0, increments every 5 turns
    int score;                      //The hunters' score starting at 366
    Players player[NUM_PLAYERS];    //5 structs containing the players data
    PlayerMessage messages[MESSAGE_SIZE]; //messages here or in player data ? - akeef
};

struct players {
    int health;
    char** trail;  //Array of arrays containing the last 5 moves for dracula; initialised;
    int trailSize;  
 };

//Prints out a given gameView for debugging
void printView(GameView g, char * pastPlays){
    printf("Turn = %d\n", g->turn);
    printf("Round = %d\n", g->round);
    printf("Score = %d\n\n", g->score);

    int i;
    for (i = 0; i <= PLAYER_DRACULA; i++){
        printf("Player %d health = %d\n", i, g->player[i]->health);
        int j;
        for (j=0; g->player[i]->trail[j] != NULL; j++){
            printf("Player %d trail[%d] = %s\n", i, j, g->player[i]->trail[j]);
        }
        printf("\n");
    }

    printf("Pastplays = %s\n", pastPlays);
}

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
static int setScore( char * pastPlays){
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

static int setTrail(GameView g, int player, char * pastPlays){
    int i, trailCount=0;
    for (i = 0; pastPlays[i] != '\0'; i++){
        //printf("%c", pastPlays[i]);
        if(i%TRAIL_SECTION_LENGTH==0 && nameEqChar(player, pastPlays[i])){
            g->player[player]->trail[trailCount] = malloc(8*sizeof(char));
            //printf("pastPlays = %s\n", pastPlays);
            //printf("Here\n");
            int j;
            for (j = 0; j < 7; j++) g->player[player]->trail[trailCount][j] = pastPlays[i+j];
            g->player[player]->trail[trailCount][7] = '\0';
            trailCount++;
        }
    }
    return trailCount;
}

//Determines if two chars are for a sea/ocean

static int trueSea(char first, char second){
    char * location = malloc(3*sizeof(char));
    location[0] = first;
    location[1] = second;
    if (abbrevToID(location) == ADRIATIC_SEA) return 1; //Adreatic Sea
    if (abbrevToID(location) == ATLANTIC_OCEAN) return 1; //Atlantic Ocean
    if (abbrevToID(location) == BAY_OF_BISCAY) return 1; //Bay of Biscay
    if (abbrevToID(location) == BLACK_SEA) return 1; //Black Sea
    if (abbrevToID(location) == ENGLISH_CHANNEL) return 1; //English Channel
    if (abbrevToID(location) == IONIAN_SEA) return 1; //Ionian Sea
    if (abbrevToID(location) == IRISH_SEA) return 1; //Irish Sea
    if (abbrevToID(location) == MEDITERRANEAN_SEA) return 1; //Mediterranean Sea
    if (abbrevToID(location) == NORTH_SEA) return 1; //North Sea
    if (abbrevToID(location) == TYRRHENIAN_SEA) return 1; //Tyhrennian Sea
    if (first == 'S' && second == '?') return 1; //SEA_UNKNOWN
    return 0;
}

//A function that determines if dracula doubled back
static int doubledBack(char * playerTrail){
    if (playerTrail[1] == 'D' && playerTrail[2] >= '1' && playerTrail[2] <= '5'){
        return playerTrail[2] - '0';
    }
    return 0;
}

//A function that determines the score for each of the characters
static int setHealth(GameView g, char * pastPlays, int player){
    //printView(g, pastPlays);
    char ** playerTrail = g->player[player]->trail;
    int health = 0;
    if (player >= PLAYER_LORD_GODALMING && player <= PLAYER_MINA_HARKER){
        health = GAME_START_HUNTER_LIFE_POINTS;

        int i;
        for (i = 0; playerTrail[i] != NULL; i++){

            int m;
            for (m = 3; m < 7;m++){
                if (playerTrail[i][m] == 'T') health -= LIFE_LOSS_TRAP_ENCOUNTER;//trap
                if (playerTrail[i][m] == 'D') health -= LIFE_LOSS_DRACULA_ENCOUNTER;//dracula
            }

            if (i != 0 && playerTrail[i][1] == playerTrail[i-1][1] && playerTrail[i][2] == playerTrail[i-1][2]) health += LIFE_GAIN_REST;//rest

            if (health <= 0) health = 9;
        }

    } else {
        health = GAME_START_BLOOD_POINTS;

        int j;
        for (j = 0; playerTrail[j] != NULL; j++){
            //hunter
            int DB = doubledBack(playerTrail[j]);
            if (DB && j != 0){
                if (trueSea(playerTrail[j-DB][1], playerTrail[j-DB][2])) health-= LIFE_LOSS_SEA;
            }
            if (trueSea(playerTrail[j][1], playerTrail[j][2])) health -= LIFE_LOSS_SEA;//sea TODO work out if C?
            if (playerTrail[j][1] == 'C' && playerTrail[j][2] == 'D') health += LIFE_GAIN_CASTLE_DRACULA;//castle
        }

        int k;
        for (k = 0; pastPlays[k] != '\0'; k++){
            if (k%8==0 && !nameEqChar(pastPlays[k], PLAYER_DRACULA)){
                int n;
                for (n = 3; n < 7; n++){
                    if (pastPlays[k+n] == 'D')health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }
        }
    }

    return health;
}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    
    //Generate Map
    gameView->gameMap = newMap(); //ADT handles locations of europe
    
    //intialise game variables
    //printf("Initialising Turn\n");
    gameView->turn = setTurn(pastPlays);
    //printf("Initialising Round\n");
    gameView->round = (gameView->turn -1)/5;
    //printf("Initialising Score\n");
    gameView->score = setScore(pastPlays); 
    
    //intitialise hunters
    int i;
    for (i = 0; i < PLAYER_DRACULA; i++){
        //printf("Initialising player %d\n", i);
        gameView->player[i] = malloc(sizeof(struct players)); 
        //printf("Initialising trail %d\n", i);
        gameView->player[i]->trail = malloc(7*GAME_START_SCORE);
        //printf("Initialising set trail %d\n", i);
        gameView->player[i]->trailSize = setTrail(gameView, i, pastPlays);
        if (gameView->player[i]->trail[gameView->player[i]->trailSize] != NULL) gameView->player[i]->trail[gameView->player[i]->trailSize] = NULL;
        //printf("Initialising health %d\n", i);
        gameView->player[i]->health = setHealth(gameView, pastPlays, i);
    }
    
    //initialise dracula
        //printf("Initialising player %d\n", i);
    gameView->player[PLAYER_DRACULA] = malloc(sizeof(struct players)); 
        //printf("Initialising trail %d\n", i);
    gameView->player[PLAYER_DRACULA]->trail = malloc(7*GAME_START_SCORE);
        //printf("Initialising set trail %d\n", i);
    gameView->player[PLAYER_DRACULA]->trailSize = setTrail(gameView, PLAYER_DRACULA, pastPlays);
    if (gameView->player[i]->trail[gameView->player[PLAYER_DRACULA]->trailSize] != NULL) gameView->player[i]->trail[gameView->player[PLAYER_DRACULA]->trailSize] = NULL;

        //printf("Initialising health %d\n", i);
    gameView->player[PLAYER_DRACULA]->health = setHealth(gameView, pastPlays, PLAYER_DRACULA);
    //TODO gameView->messages = messages;

    //printView(gameView, pastPlays);
    
    return gameView;
}
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    int i;
    for (i = 0; i <= PLAYER_DRACULA; i++){
        int j;
        for(j=0; toBeDeleted->player[i]->trail[j] != NULL; j++){
            //printf("To be delet%s\n", toBeDeleted->player[i]->trail[j]);
            free(toBeDeleted->player[i]->trail[j]);
            //printf("Freed player[%d] trail[%d]\n",i, j);
        }
        free(toBeDeleted->player[i]->trail);
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
    if (i < 0) return UNKNOWN_LOCATION;
    char * location = malloc(3*sizeof(char));
    location[0] = currentView->player[player]->trail[i][1];
    location[1] = currentView->player[player]->trail[i][2];
    location[2] = '\0';

    if (location[1] == '?') {
        if (location[0] == 'C') return CITY_UNKNOWN;//Drac Unknown
        if (location[0] == 'S') return SEA_UNKNOWN;
    } else if (player == PLAYER_DRACULA && location[0] == 'D' && location[1] >= '1' && location[1] <= '5'){
        return DOUBLE_BACK_1 - 1 + location[1]-'0';
    } else {
        return abbrevToID(location);
    }
    return abbrevToID(location);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
// Slighlty adjusted this function -J
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int i, n = 0;
    char * location = malloc(3*sizeof(char));
    for(i = currentView->player[player]->trailSize -1; (i >=0 && n < 6); i--){
        location[0] = currentView->player[player]->trail[i][1];
        location[1] = currentView->player[player]->trail[i][2];
        location[2] = '\0';
        //printf("%s\n", location);
        if (location[1] == '?') {
            if (location[0] == 'C') trail[n]= CITY_UNKNOWN;//Drac Unknown
            if (location[0] == 'S') trail[n] = SEA_UNKNOWN;
        } else if (player == PLAYER_DRACULA && location[0] == 'D' && location[1] >= '1' && location[1] <= '5'){
            trail[n]= DOUBLE_BACK_1 - 1 + location[1]-'0';
        } else {
            trail[n] = abbrevToID(location);
        }
        n++;
    }
    //printf("%d %d %d %d %d\n", trail[0], trail[1], trail[2], trail[3], trail[4]);
    int j;
    for (j = n; j < TRAIL_SIZE; j++){
        trail[j] = UNKNOWN_LOCATION;
    }
    //printf("%d %d %d %d %d\n", trail[0], trail[1], trail[2], trail[3], trail[4]);

}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea){
    
    //Total connections
    int totalCount = numOfConnections(currentView->gameMap, from);
    //Amount of connections to be returned
    //Starts at 1 so as to include starting vertex
    int count = 1;
    
    //Stores data for connections
    LocationID *connections = malloc(NUM_MAP_LOCATIONS);
    TransportID *transport = malloc(NUM_MAP_LOCATIONS);

    getConnections(currentView->gameMap, from, connections, transport);
    
    //Initiliase array of values to be returned
    int *locations = malloc(NUM_MAP_LOCATIONS);

    //Set first connection as the starting vertex
    locations[0] = from;
    
    //Iterate through connections and find those of the requested types
    int i;
    for (i = 1; i <= totalCount; i++) {
        //Add connection to locations[] if it is of a requested type
        if (road && (transport[i] == ROAD || transport[i] == ANY)) {
            locations[count] = connections[i];
            count++;
        } else if (rail && (transport[i] == RAIL || transport[i] == ANY)) {
            locations[count] = connections[i];
            count++;
        } else if (sea && (transport[i] == SEA || transport[i] == ANY)) {
            locations[count] = connections[i];
            count++;
        }
    }
    
    *numLocations = count;
    //printf("%d\n", *numLocations);
    return locations;
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
