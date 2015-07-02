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
enum dificulty{none,easy,normal,hard};
 
typedef struct{
    SDL_Rect Pos;
    int colorCode;
}Egg;
 
typedef struct{
    int velocity;
    int typeCode;
    SDL_Texture *busterTexture;
    SDL_Rect Pos;
}Buster;
 
bool sdlStartup();
bool sdlMediaStartup(int dificulty);
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
bool verifyCollide(Buster *buster, Egg **matrix);
void destroyEggs(int i, int j, int bt, int et);
 
Egg **matrix = NULL;
SDL_Window *window = NULL;
SDL_Event events;
SDL_Texture *texture = NULL, *background = NULL;
SDL_Renderer *renderer = NULL;
Mix_Music *bgMusic = NULL;
Buster *buster = NULL;
bool move = true;
int radius;

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
 
bool sdlMediaStartup(int dificulty){
    bool success;

    switch(dificulty){
        case easy:
            background = IMG_LoadTexture(renderer,"Img/BG_EASY.jpg");
            radius = 250;
            break;
        case normal:
            background = IMG_LoadTexture(renderer,"Img/BG_NORMAL.jpg");
            radius = 200;
            break;
        case hard:
            background = IMG_LoadTexture(renderer,"Img/BG_HARD.jpg");
            radius = 150;
            break;

    }

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
    float circleEQ;
 
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
            (matrix[i][j]).Pos.x = assignPosition(i,j);
            (matrix[i][j]).Pos.y = EGG_TILE_SIZE*i;
            (matrix[i][j]).Pos.w = EGG_TILE_SIZE;
            (matrix[i][j]).Pos.h = EGG_TILE_SIZE;
            
 
            circleEQ = pow(((matrix[i][j]).Pos.x)-(SCREEN_WIDTH/2),2) + 
            pow((((matrix[i][j]).Pos.y)-(SCREEN_HEIGHT/2)),2);
 
            if(circleEQ > pow(radius,2)){
                SDL_RenderCopy(renderer, texture, NULL, &(matrix[i][j]).Pos);
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
        result = (SCREEN_HEIGHT*0.001)+((EGG_TILE_SIZE/2))*((rowNumber+1)%2)+(columnNumber*EGG_TILE_SIZE);
    }
 
    return result;
}
 
void drawBuster(){
    buster = (Buster*) malloc(sizeof(Buster));
    buster->typeCode = randomizeBuster();
 
    switch(buster->typeCode){
                case mint:
                buster->busterTexture = IMG_LoadTexture(renderer, "Img/M.png"); 
                break;
                case fire:
                buster->busterTexture = IMG_LoadTexture(renderer, "Img/F.png"); 
                break;
                case water:
                buster->busterTexture = IMG_LoadTexture(renderer, "Img/W.png"); 
                break;
                case ice:
                buster->busterTexture = IMG_LoadTexture(renderer, "Img/I.png"); 
                break;
                case magic:
                buster->busterTexture = IMG_LoadTexture(renderer, "Img/MA.png"); 
                break;
    } 
 
    buster->velocity = 16;
 
    buster->Pos.x = SCREEN_WIDTH/2;
    buster->Pos.y = SCREEN_HEIGHT/2;
    buster->Pos.w = EGG_TILE_SIZE;
    buster->Pos.h = EGG_TILE_SIZE;
 
    SDL_RenderCopy(renderer, buster->busterTexture, NULL, &(buster->Pos));
}
 
void moveBuster(){

	const Uint8 *press = SDL_GetKeyboardState(NULL);

	if(press[SDL_SCANCODE_UP]){
		if(press[SDL_SCANCODE_LEFT]){
			buster->Pos.y -= buster->velocity; 
			buster->Pos.x -= buster->velocity;
		}

		if(press[SDL_SCANCODE_RIGHT]){
			buster->Pos.y -= buster->velocity; 
			buster->Pos.x += buster->velocity;
		}
		buster->Pos.y -= buster->velocity; 
        move = true;
	}

	if(press[SDL_SCANCODE_DOWN]){
		if(press[SDL_SCANCODE_LEFT]){
			buster->Pos.y += buster->velocity; 
			buster->Pos.x -= buster->velocity;
		}

		if(press[SDL_SCANCODE_RIGHT]){
			buster->Pos.y += buster->velocity; 
			buster->Pos.x += buster->velocity;
		}
		buster->Pos.y += buster->velocity;
        move = true; 
	}

	if(press[SDL_SCANCODE_LEFT]){
		buster->Pos.x -= buster->velocity;
        move = true; 
	}

	if(press[SDL_SCANCODE_RIGHT]){
		buster->Pos.x += buster->velocity;
        move = true;
	}

	if(buster->Pos.x < 0 ){
		buster->Pos.x += buster->velocity;
	}
	else if(buster->Pos.x + EGG_TILE_SIZE > SCREEN_WIDTH){
		buster->Pos.x -= buster->velocity;
	}

	if(buster->Pos.y < 0){
		buster->Pos.y += buster->velocity;
	}
	else if(buster->Pos.y + EGG_TILE_SIZE > SCREEN_HEIGHT){
		buster->Pos.y -= buster->velocity;
	}


    if(move == true){
        reDraw();
        if(verifyCollide(buster,matrix) == true){
            //reDraw();
        }
        move = false;
    }
}
 
 
void reDraw(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    drawEggs(renderer,matrix);
    SDL_RenderCopy(renderer, buster->busterTexture, NULL, &(buster->Pos));
    SDL_RenderPresent(renderer);
}

int randomizeBuster(){
    int seed = time(NULL);
    int luck = 100, generated;
    srand(seed);

    generated = (rand()%9);
    while(generated < 5){
        generated = (rand()%9);
    }
    

    while(generated == magic && luck > 0){
        while(generated < 5){
            generated = (rand()%9);
        }
        luck--;
    }

    return generated;
}

bool verifyCollide(Buster *buster, Egg **matrix){
    int i,j;
    system("clear");
    printf("BX: %d\n",(buster->Pos.x + 0) );
    printf("BY: %d\n",(buster->Pos.y + 0) );
    printf("%d\n",buster->typeCode);

    for (i = 0; i < ROWS; i++){
        for (j = 0; j < COLUMNS; j++){
            if((matrix[i][j]).colorCode != 0){
                if( SDL_HasIntersection( &(buster->Pos), &((matrix[i][j]).Pos))){
                    printf("Collide\n");
                    destroyEggs(i,j,buster->typeCode,buster->typeCode-4);
                    return true;
                }
            }

        }
    }
    return false;
}

void destroyEggs(int i, int j,int bustertype, int eggtype){
	
	bool singleCollide = true;
	
	if(buster->typeCode == bustertype){
		if((matrix[i][j]).colorCode == eggtype){

				if(j != 0){
					if(((matrix[i][j]).colorCode == (matrix[i][j-1]).colorCode)){
						(matrix[i][j]).colorCode = nil;
						(matrix[i][j-1]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(j != 18){
					if((matrix[i][j]).colorCode == (matrix[i][j+1]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i][j+1]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 0){
					if((matrix[i][j]).colorCode == (matrix[i-1][j]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i-1][j]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 19){
					if((matrix[i][j]).colorCode == (matrix[i+1][j]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i+1][j]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 0 && j != 0){
					if((matrix[i][j]).colorCode == (matrix[i-1][j-1]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i-1][j-1]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 19 && j != 0){
					if((matrix[i][j]).colorCode == (matrix[i+1][j-1]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i+1][j-1]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 19 && j != 18){
					if((matrix[i][j]).colorCode == (matrix[i+1][j+1]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i+1][j+1]).colorCode = nil;
						singleCollide = false;
					}
				}

				if(i != 0 && j != 18){
					if((matrix[i][j]).colorCode == (matrix[i-1][j+1]).colorCode){
						(matrix[i][j]).colorCode = nil;
						(matrix[i-1][j+1]).colorCode = nil;
						singleCollide = false;
					}
				}

			if(singleCollide == false){
				free(buster);
				buster = NULL;
				drawBuster();
			}

		}
	}
}
