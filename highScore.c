#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Libs/mainLib.h"
#include "Libs/libget.h" // getint getstr getfloat

SDL_Event keyPress;
SDL_Window* w1 = NULL;

typedef struct{
	char* name;
	int score;
}player;

FILE* fr; //for reading
FILE* fw; //for writing

int getScore();
player* fillScores();
void printTable( player* prntTable );

int main( int argc, char** argv ){
	int i = 9, aux;
	char* staux;
	player player1;
	player* highScore;

	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO);
	w1 = SDL_CreateWindow( "Score", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0 );
	fr = fopen("score.t", "r+");
	if( fr == NULL ){
		highScore = fillScores();	
	}else{           
		highScore = malloc( sizeof( player ) * 10 );
		for( i = 0; i < 10; i++ ){
			highScore[i].name = malloc( 10 );
			fread( highScore[i].name, 10, 1, fr );
			fread( &highScore[i].score, sizeof(int)*10, 1, fr);
			//se lee por variable y por los miembro de estrcutura
		}
		fclose( fr );
		printTable( highScore );
	}
	player1.score = getScore();
	SDL_DestroyWindow( w1 );
	//score sorting
	if( player1.score > highScore[9].score){
		highScore[9].score = player1.score;
		for( i = 9; i > 0; i-- ){
			if( highScore[i].score > highScore[ i - 1 ].score  ){
				aux = highScore[ i - 1 ].score;
				staux = highScore[ i - 1 ].name;
				highScore[ i - 1 ].score = highScore[i].score;
				highScore[ i - 1 ].name =  highScore[i].name;
				highScore[i].score = aux;
				highScore[i].name = staux;
			}else break;
		}
		printf("Nuevo HighScore! Ingrese su nombre: ");
		highScore[i].name = getstr();
	}
	//ends sorting
	printTable( highScore );
	fw = fopen( "score.t", "wb" );
	for( i = 0; i < 10; i++ ){
		fwrite( highScore[i].name, 10, 1, fw);
		fwrite( &highScore[i].score, sizeof(int)*10, 1, fw);
	}
	fclose( fw );
	return 0;
}

int getScore(){
	player p1 = { "aa", 0 };
	int quit = 0;
	while( !quit ){
		if( SDL_WaitEvent( &keyPress ) ){
			if( keyPress.type == SDL_QUIT ){
				SDL_Quit();
				quit = 1;
			}
			if( keyPress.type == SDL_KEYDOWN ){
				if( keyPress.key.keysym.scancode == SDL_SCANCODE_SPACE ){
					p1.score += 100;
					printf("Score: %d\n", p1.score);
				}else{
					return p1.score;
				}
			}
		}
	}
	return p1.score;
}

player* fillScores(){
	int i; char* aux;
	player* array = malloc( sizeof(player) * 10 );
	for( i = 9; i >= 0; i-- ){
		array[i].name = malloc( 10 );
		array[i].name = "";
		array[i].score = 0;
	}
	return array;
}

void printTable( player* prntTable ){
	int i; char* aux;
	for( i = 0; i  < 10; i++ ){
		printf("%s -- score: %d\n", prntTable[i].name, prntTable[i].score);
	}
}
