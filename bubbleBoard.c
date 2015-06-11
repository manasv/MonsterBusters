/*#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*#define SCREEN_WIDTH 640 
#define SCREEN_HEIGHT 480*/
#define ROWS 5
#define COLUMNS 15

typedef struct{
	int colorCode;	
}Bubble;

void allocateMatrix(Bubble ***matrix);
Bubble* fillMatrix(Bubble **matrix);
void showMatrix(Bubble ** matrix);

int main(int argc, char const *argv[]){

	Bubble **bMatrix = NULL;

	allocateMatrix(&bMatrix);
	fillMatrix(bMatrix);
	showMatrix(bMatrix);

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
			(*(matrix+i))[j].colorCode = rand()%3;
		}
	}
	return *matrix;
}

void showMatrix(Bubble ** matrix){
        int i ,j;
         
        for(i = 0 ; i < ROWS; i++){
            for( j=0 ; j< COLUMNS; j++){
                    printf(" |%d| ",(matrix[i][j]).colorCode);
            }
            printf("\n");
        }
}
