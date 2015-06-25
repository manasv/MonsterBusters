#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Libs/mainLib.h"
#include "Libs/libget.h" // getint getstr getfloat

SDL_Event keyPress;
SDL_Window* w1 = NULL;
Mix_Chunk* beepy = NULL;


typedef struct{
	char* name;
	int score;
}player;

FILE* fr; FILE* fw; //for reading and writing
FILE* frb; FILE* fwb; //for reading and writing binary

int getScore();
player* fillScores();
void printTable( player* prntTable );

int main( int argc, char** argv ){
	int i = 9, aux;
	char* staux = malloc( 10 );
	player player1;
	player* highScore;

	SDL_Init( SDL_INIT_EVERYTHING);
	w1 = SDL_CreateWindow( "Score", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0 );
	frb = fopen("./Saves/scores.t", "rb");
	fr = fopen("./Saves/players.t", "r");
	if( frb == NULL ){
		highScore = fillScores();	
	}else{           
		highScore = malloc( sizeof( player ) * 10 );
		for( i = 0; i < 10; i++ ){
			highScore[i].name = malloc( 10 );
			fscanf( fr, "%s", staux );
			strcpy( highScore[i].name, staux );
			fread( &highScore[i].score, sizeof(int), 1, frb);
			//se lee por variable y por los miembro de estrcutura
		}
		fclose( frb );
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
		highScore[i].name = getname();
	}
	//ends sorting
	printTable( highScore );
	fwb = fopen( "./Saves/scores.t", "wb" );
	fw = fopen( "./Saves/players.t", "w" );
	for( i = 0; i < 10; i++ ){
		fprintf(fw,"%s\n",highScore[i].name);
		fwrite( &highScore[i].score, sizeof(int), 1, fwb);
	}
	fclose( fw );
	fclose( fwb );
	free( highScore );
	Mix_Quit();
	SDL_Quit();
	return 0;
}

int getScore(){
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1){
		exit(0);
	}
	beepy = Mix_LoadWAV( "Sound/points.wav" );
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
				Mix_PlayChannel( -1, beepy, 0 );
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
		array[i].name = "Empty";
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
