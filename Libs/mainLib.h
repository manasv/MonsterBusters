#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "macros.h"
 
typedef enum bool{false,true}bool;
enum tileCode{nil,cthulhu,ghoulbeast,kumonga,seaserpent,mint,fire,water,ice,magic};
 
typedef struct{
    int x;
    int y;
    int colorCode;
}Egg;
 
typedef struct{
    int velocity;
    int posX;
    int posY;
    int typeCode;
    SDL_Texture *busterTexture;
    SDL_Rect Pos;
    SDL_Rect Rect;
}Buster;
 
bool sdlStartup();
bool sdlMediaStartup();
void closeALL();
void allocateMatrix(Egg ***matrix);
Egg* fillMatrix(Egg **matrix);
void showMatrix(Egg **matrix);
void freeEggs(Egg **matrix, int rows);
void drawEggs(SDL_Renderer *renderer,Egg **matrix);
int assignPosition(int rowNumber, int columnNumber);
void drawBuster();
void moveBuster();
void reDraw();
int randomizeBuster();
void verifyCollide(Buster buster, Egg **matrix);
 
Egg **matrix = NULL;
SDL_Window *window = NULL;
SDL_Event events;
SDL_Texture *texture = NULL, *background = NULL, *eggDestroyer = NULL;
SDL_Renderer *renderer = NULL;
Mix_Music *bgMusic = NULL;
Buster buster;
bool move = true;

bool sdlStartup(){
 
    bool success = true;
 
 
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else{
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" ) ){
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
            (matrix[i][j]).colorCode = (rand()%5);
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
 
void drawBuster(){
    buster.typeCode = randomizeBuster();
 
    switch(buster.typeCode){
                case mint:
                buster.busterTexture = IMG_LoadTexture(renderer, "Img/M.png"); 
                break;
                case fire:
                buster.busterTexture = IMG_LoadTexture(renderer, "Img/F.png"); 
                break;
                case water:
                buster.busterTexture = IMG_LoadTexture(renderer, "Img/W.png"); 
                break;
                case ice:
                buster.busterTexture = IMG_LoadTexture(renderer, "Img/I.png"); 
                break;
                case magic:
                buster.busterTexture = IMG_LoadTexture(renderer, "Img/MA.png"); 
                break;
    } 
 
    buster.velocity = 16;
 
    buster.Pos.x= SCREEN_WIDTH/2;
    buster.Pos.y= SCREEN_HEIGHT/2;
    buster.Pos.w= EGG_TILE_SIZE;
    buster.Pos.h= EGG_TILE_SIZE;
 
    buster.posX = buster.Pos.x;
    buster.posY = buster.Pos.y;
    buster.Rect.x = 0;
    buster.Rect.y = 0;
    buster.Rect.w = EGG_TILE_SIZE;
    buster.Rect.h = EGG_TILE_SIZE;
 
    SDL_RenderCopy(renderer, buster.busterTexture, NULL, &(buster.Pos));
}
 
void moveBuster(){

	const Uint8 *press = SDL_GetKeyboardState(NULL);

	if(press[SDL_SCANCODE_UP]){
		if(press[SDL_SCANCODE_LEFT]){
			buster.Pos.y -= buster.velocity/2; 
			buster.Pos.x -= buster.velocity/2;
		}

		if(press[SDL_SCANCODE_RIGHT]){
			buster.Pos.y -= buster.velocity/2; 
			buster.Pos.x += buster.velocity/2;
		}
		buster.Pos.y -= buster.velocity; 
        move = true;
	}

	if(press[SDL_SCANCODE_DOWN]){
		if(press[SDL_SCANCODE_LEFT]){
			buster.Pos.y += buster.velocity/2; 
			buster.Pos.x -= buster.velocity/2;
		}

		if(press[SDL_SCANCODE_RIGHT]){
			buster.Pos.y += buster.velocity/2; 
			buster.Pos.x += buster.velocity/2;
		}
		buster.Pos.y += buster.velocity;
        move = true; 
	}

	if(press[SDL_SCANCODE_LEFT]){
		buster.Pos.x -= buster.velocity;
        move = true; 
	}

	if(press[SDL_SCANCODE_RIGHT]){
		buster.Pos.x += buster.velocity;
        move = true;
	}

	if(buster.Pos.x < 0 ){
		buster.Pos.x += buster.velocity;
	}
	else if(buster.Pos.x + EGG_TILE_SIZE > SCREEN_WIDTH){
		buster.Pos.x -= buster.velocity;
	}

	if(buster.Pos.y < 0){
		buster.Pos.y += buster.velocity;
	}
	else if(buster.Pos.y + EGG_TILE_SIZE > SCREEN_HEIGHT){
		buster.Pos.y -= buster.velocity;
	}
    //verifyCollide(buster,matrix);
    if(move == true){
        reDraw();
        move = false;
    }
}
 
 
void reDraw(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    drawEggs(renderer,matrix);
    SDL_RenderCopy(renderer, buster.busterTexture, NULL, &(buster.Pos));
    SDL_RenderPresent(renderer);
}

int randomizeBuster(){
    int luck = 5, generated;

    generated = (rand()%4)+6;

    while(generated == magic && luck > 0){
        generated = (rand()%4)+6;
        luck--;
    }

    return generated;
}

void verifyCollide(Buster buster, Egg **matrix){
    int i,j;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            if( (buster.Pos.x + EGG_TILE_SIZE) > (matrix[i][j]).x){
                buster.Pos.x += buster.velocity;
            }     
        }
    }

}
