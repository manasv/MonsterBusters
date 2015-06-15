#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BUBBLE_SIZE 32
#define ROWS (SCREEN_HEIGHT/BUBBLE_SIZE)-8
#define COLUMNS (SCREEN_WIDTH/BUBBLE_SIZE)-1

typedef enum bool{false,true}bool;
enum eggType{vine,pinterest,dropbox,imdb};

typedef struct{
	int x;
	int y;
	int colorCode;
}Bubble;

void allocateMatrix(Bubble ***matrix);
Bubble* fillMatrix(Bubble **matrix);
void showMatrix(Bubble **matrix);
void freeBubbles(Bubble **matrix, int rows);
void drawBubbles(SDL_Renderer *renderer,Bubble **matrix);
int assignPosition(int rowNumber, int columnNumber);


int main(int argc, char const *argv[]){

	Bubble **bMatrix = NULL;
	bool active = true;

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Event events;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("The Bubble Game",SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	IMG_Init(IMG_INIT_PNG);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	screenSurface = SDL_GetWindowSurface(window);
	SDL_SetRenderDrawColor(renderer, 23, 0, 82, 1);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	allocateMatrix(&bMatrix);
	fillMatrix(bMatrix);
	showMatrix(bMatrix);

	

	while(active) {
		drawBubbles(renderer,bMatrix);

		while( SDL_PollEvent( &events ) != 0 ){
			if( events.type == SDL_QUIT ){
				active = false;
				freeBubbles(bMatrix,ROWS);
			}
		}
	}

	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}

void allocateMatrix(Bubble ***matrix){
	int i;

	*matrix = (Bubble **) malloc(sizeof(Bubble *)*ROWS);

	if( *matrix != NULL){
		for( i = 0 ; i < ROWS ; i++){
			(*matrix)[i] = (Bubble *) malloc(sizeof(Bubble)*COLUMNS);
		}
	}
}

Bubble* fillMatrix(Bubble **matrix){
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

void showMatrix(Bubble **matrix){
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

void freeBubbles(Bubble **matrix, int rows){
	int i;
	for(i=0;i<rows;i++){
		free(matrix[i]);
		matrix[i] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

void drawBubbles(SDL_Renderer *renderer,Bubble **matrix){
	int i,j;

	SDL_Texture* texture = NULL;
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
			newPosition.y = BUBBLE_SIZE*i;
			newPosition.w = BUBBLE_SIZE;
			newPosition.h = BUBBLE_SIZE;
			(matrix[i][j]).x = newPosition.x;
			(matrix[i][j]).y = newPosition.y;

			SDL_RenderCopy(renderer, texture, NULL, &newPosition);
			SDL_RenderPresent(renderer);
		}
	}
}

int assignPosition(int rowNumber, int columnNumber){
	int result;

	if(rowNumber%2 == 0){
		result = ((BUBBLE_SIZE/2)+10)*((rowNumber+1)%2)+(columnNumber*BUBBLE_SIZE);
	}
	else{
		result = 6+((BUBBLE_SIZE/2))*((rowNumber+1)%2)+(columnNumber*BUBBLE_SIZE);
	}

	return result;
}
