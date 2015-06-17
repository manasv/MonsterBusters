#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "macros.h"

typedef enum bool{false,true}bool;
enum eggType{vine,pinterest,dropbox,imdb};

typedef struct{
	int x;
	int y;
	int colorCode;
}Egg;

void allocateMatrix(Egg ***matrix);
Egg* fillMatrix(Egg **matrix);
void showMatrix(Egg **matrix);
void freeEggs(Egg **matrix, int rows);
void drawEggs(SDL_Renderer *renderer,Egg **matrix);
int assignPosition(int rowNumber, int columnNumber);

SDL_Window* window = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* background = NULL;
SDL_Event events;
SDL_Renderer* renderer = NULL;
Mix_Music* bgMusic = NULL;

void allocateMatrix(Egg ***matrix){
	int i;

	*matrix = (Egg **) malloc(sizeof(Egg *)*ROWS);

	if( *matrix != NULL){
		for( i = 0 ; i < ROWS ; i++){
			(*matrix)[i] = (Egg *) malloc(sizeof(Egg)*COLUMNS);
		}
	}
}

Egg* fillMatrix(Egg **matrix){
	int seed = time(NULL);
	srand(seed);

	int i,j;
	for(i=0 ; i < ROWS; i++){
		for(j=0; j<COLUMNS; j++){
			(*(matrix+i))[j].colorCode = rand()%4;
		}
	}
	return *matrix;
}

void showMatrix(Egg **matrix){
	int i ,j;

	for(i = 0 ; i < ROWS; i++){
		for( j=0 ; j< COLUMNS; j++){
			if(j == 0){
				printf("|");
			}
			printf("%d|",(matrix[i][j]).colorCode);
		}
		printf("\n");
	}
}

void freeEggs(Egg **matrix, int rows){
	int i;
	for(i=0;i<rows;i++){
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

void drawEggs(SDL_Renderer *renderer,Egg **matrix){
	int i,j;

	SDL_Rect newPosition;

	for(i=0;i<ROWS;i++){
		for(j=0;j<COLUMNS;j++){

			switch(matrix[i][j].colorCode){
				case vine:
				texture = IMG_LoadTexture(renderer, "Img/0.png");
				break;
				case pinterest:
				texture = IMG_LoadTexture(renderer, "Img/1.png");
				break;
				case dropbox:
				texture = IMG_LoadTexture(renderer, "Img/2.png");
				break;
				case imdb:
				texture = IMG_LoadTexture(renderer, "Img/3.png");
				break;
			} 
			newPosition.x = assignPosition(i,j);
			newPosition.y = EGG_TILE_SIZE*i;
			newPosition.w = EGG_TILE_SIZE;
			newPosition.h = EGG_TILE_SIZE;
			(matrix[i][j]).x = newPosition.x;
			(matrix[i][j]).y = newPosition.y;


			SDL_RenderCopy(renderer, texture, NULL, &newPosition);
			/*SDL_RenderPresent(renderer);*/ //Testing where is better to present the renderer
		}
	}
}

int assignPosition(int rowNumber, int columnNumber){
	int result;

	if(rowNumber%2 == 0){
		result = ((EGG_TILE_SIZE/2)+10)*((rowNumber+1)%2)+(columnNumber*EGG_TILE_SIZE);
	}
	else{
		result = 6+((EGG_TILE_SIZE/2))*((rowNumber+1)%2)+(columnNumber*EGG_TILE_SIZE);
	}

	return result;
}