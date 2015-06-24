#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "macros.h"

typedef enum bool{false,true}bool;
enum eggType{nil,cthulhu,ghoulbeast,kumonga,seaserpent};

typedef struct{
	int x;
	int y;
	int colorCode;
}Egg;

bool sdlStartup();
bool sdlMediaStartup();
void closeALL();
void allocateMatrix(Egg ***matrix);
Egg* fillMatrix(Egg **matrix);
void showMatrix(Egg **matrix);
void freeEggs(Egg **matrix, int rows);
void drawEggs(SDL_Renderer *renderer,Egg **matrix);
int assignPosition(int rowNumber, int columnNumber);
void drawAntiEgg();
void moveAntiEggEvent();
void reDraw();

Egg **matrix = NULL;
SDL_Window *window = NULL;
SDL_Event events;
SDL_Texture *texture = NULL, *background = NULL, *eggDestroyer = NULL;
SDL_Renderer *renderer = NULL;
Mix_Music *bgMusic = NULL;
SDL_Rect antiEggPos;
bool pause = false;

bool sdlStartup(){

	bool success = true;


	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		window = SDL_CreateWindow(GAME_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
		if( window == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				SDL_SetRenderDrawColor(renderer, 23, 0, 82, 1);
				
				int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
					printf( "SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool sdlMediaStartup(){
	bool success;

	background = IMG_LoadTexture(renderer,"Img/background.jpg");
	SDL_RenderCopy(renderer, background, NULL, NULL);

	bgMusic = Mix_LoadMUS("Sound/music.mp3");
	if( bgMusic == NULL ){
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	Mix_PlayMusic(bgMusic,-1);

	return success;
}

void closeALL(){
	freeEggs(matrix,ROWS);

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(background);
	texture = NULL;
	background = NULL;

	Mix_CloseAudio();
	Mix_FreeMusic(bgMusic); 
	bgMusic = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

}

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
			(matrix[i][j]).colorCode = (rand()%4)+1;
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
	float circleEQ, radius = 200;

	SDL_Rect newPosition;

	for(i=0;i<ROWS;i++){
		for(j=0;j<COLUMNS;j++){

			switch(matrix[i][j].colorCode){
				case nil:
				/*texture = IMG_LoadTexture(renderer, "Img/trans.png");*/
				break;
				case cthulhu:
				texture = IMG_LoadTexture(renderer, "Img/C.png");
				break;
				case ghoulbeast:
				texture = IMG_LoadTexture(renderer, "Img/G.png");
				break;
				case kumonga:
				texture = IMG_LoadTexture(renderer, "Img/K.png");
				break;
				case seaserpent:
				texture = IMG_LoadTexture(renderer, "Img/S.png");
				break;
			} 
			newPosition.x = assignPosition(i,j);
			newPosition.y = EGG_TILE_SIZE*i;
			newPosition.w = EGG_TILE_SIZE;
			newPosition.h = EGG_TILE_SIZE;
			(matrix[i][j]).x = newPosition.x;
			(matrix[i][j]).y = newPosition.y;

			circleEQ = pow(((matrix[i][j]).x)-(SCREEN_WIDTH/2),2) + 
			pow(((matrix[i][j]).y)-(SCREEN_HEIGHT/2),2);

			if(circleEQ > pow(radius,2)){
				SDL_RenderCopy(renderer, texture, NULL, &newPosition);
			}
			else{
				(matrix[i][j]).colorCode = nil;
			}

			SDL_DestroyTexture(texture);
		}
	}
}

int assignPosition(int rowNumber, int columnNumber){
	int result;

	if(rowNumber%2 == 0){
		result = ((EGG_TILE_SIZE/2)+(SCREEN_WIDTH*0.015))*((rowNumber+1)%2)+(columnNumber*EGG_TILE_SIZE);
	}
	else{
		result = (SCREEN_WIDTH*0.01)+((EGG_TILE_SIZE/2))*((rowNumber+1)%2)+(columnNumber*EGG_TILE_SIZE);
	}

	return result;
}

void drawAntiEgg(){
	eggDestroyer = IMG_LoadTexture(renderer, "Img/F.png");

	antiEggPos.x= SCREEN_WIDTH/2;
    antiEggPos.y= SCREEN_HEIGHT/2;
    antiEggPos.w= EGG_TILE_SIZE;
    antiEggPos.h= EGG_TILE_SIZE;

    SDL_RenderCopy(renderer, eggDestroyer, NULL, &antiEggPos);
}

void moveAntiEggEvent(){
	if(pause == false){
		if( events.type == SDL_KEYDOWN && events.key.repeat == 0){
        switch( events.key.keysym.sym ){
            case SDLK_UP: antiEggPos.y -= 16; break;
            case SDLK_DOWN: antiEggPos.y += 16; break;
            case SDLK_LEFT: antiEggPos.x -= 16; break;
            case SDLK_RIGHT: antiEggPos.x += 16; break;
        	}
        	pause = true;
    	}
	}
	if(pause == true){
		reDraw();	
	}
}

void reDraw(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    drawEggs(renderer,matrix);
    SDL_RenderCopy(renderer, eggDestroyer, NULL, &antiEggPos);
    pause = false;
}