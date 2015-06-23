#include <stdio.h>
#include <unistd.h>
#include "Libs/mainLib.h"

SDL_Event keyPress;
SDL_Window* w1 = NULL;

typedef struct{
	char* player;
	unsigned int score;
}highscore;

FILE* fr; //for reading
FILE* fw; //for writing

int getScore();
highscore* fillScores();

int main( int argc, char** argv ){
	highscore player1;
	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO);
	w1 = SDL_CreateWindow( "Score", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0 );
	fr = fopen("score.t", "rb");
	if( fr == EOF ){
		fclose( fr );
		fw = fopen( "score.t", "wb" );

	}
	highscore* playerArray = fillScores();
	player1.score = getScore();
}

int getScore(){
	highscore p1 = { "aa", 0 };
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

highscore* fillScores(){
	int i;
	highscore* array = malloc( sizeof(highscore) * 10 );
	for( i = 0; i < 10; i++ ){
		array[i].player = "emmanuel";
		array[i].score = 0;
		printf( "%s, score: %d\n",array[i].player, array[i].score );
	}
	return array;
}
