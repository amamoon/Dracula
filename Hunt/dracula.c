// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

int coinFlip(void);
void LocationCopy (char *from, char *overHere);
char *idToAbbrev(LocationID id);

void decideDraculaMove(DracView gameState)
{
	// TODO ...
	// Replace the line below by something better
	if(giveMeTheRound(gameState) == 1){  registerBestPlay("CD","Welcome");//drac starts at the castle
	}else{
		//rounds >= 2
		//Subsequent Rounds
		//coin implementation here
		/*if(coinFlip() =  1) go to Gelatz
		else go to klasenberg
		(how often do we want to go back to CD ?)
		*/
		char dest[3]; //Destination var
		 
		//Go to random places
        int size , *isItPossible;
        int daRound = giveMeTheRound(gameState);
        isItPossible = whereCanIgo(gameState,&size,1,0,0);
        while(daRound > size){
            daRound = daRound % size;
        }
        LocationCopy(edges[daRound],dest);
        registerBestPlay(dest,"Nothing to find here");
		free(dest);
        return;
	}
}

void LocationCopy (char *from, char *overHere){
    int count = 0;
    overHere[0] = from[0];
    overHere[1] = from[1];
    overHere[2] = '\0';
    //free(count); double check this
}

char *idToAbbrev(LocationID id){
   assert(validPlace(id));
   return places[id].abbrev;
}

int coinFlip(void){
	int flip = rand()%2;
	if(flip == 1) return 1;
	return 0;
}
		

