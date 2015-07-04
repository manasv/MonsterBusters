#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "macros.h"
#include "highScoregen.h"
#include "arm_final.h"

typedef enum bool{false,true}bool;
enum tileCode{nil,cthulhu,ghoulbeast,kumonga,seaserpent,mint,fire,water,ice,magic};
enum dificulty{none,easy,normal,hard};
enum generated{no_generated,generated};

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
void drawEggs(SDL_Renderer *renderer,Egg **matrix,int flag);
void drawCurrentScore( SDL_Renderer* renderer, char* score_char ); // imprime Score en pantalla!
int assignPosition(int rowNumber, int columnNumber);
void drawBuster();
void moveBuster();
void reDraw(int flag);
int randomizeBuster();
bool verifyCollide(Buster *buster, Egg **matrix);
void destroyEggs(int i, int j, int bt, int et );
void theHellisComing(int dificulty);
 
Egg **matrix = NULL;
SDL_Window *window = NULL;
SDL_Event events;
SDL_Texture *texture = NULL, *background = NULL;
SDL_Texture* scoreTextureFont = NULL;
SDL_Surface* scoreTextSurface = NULL;
SDL_Rect textPos_score = { 320, 50, 0, 0 };
SDL_Renderer *renderer = NULL;
SDL_Renderer* renderer2 = NULL;
Mix_Music *bgMusic = NULL;
Buster *buster = NULL;
int score = 0; //se pasara como argumento a la funcion score_gen!
TTF_Font* scoreDisplayFont = NULL;
SDL_Color whitescore = { 255, 255, 255 };
SDL_Color blackscore = { 0, 0, 0, 155 };
char* score_char;
bool move = true;
int radius,globalDificulty;
Uint32 start,oldTime,currentTime;

bool sdlStartup(){
 
    bool success = true;
	score_char = malloc( 25 );
	sprintf( score_char, "%d", 0 );


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
            renderer2 = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
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
				TTF_Init();
				scoreDisplayFont = TTF_OpenFont( "./Fonts/PragmataPro for Powerline.ttf", 25 );
            }
        }
    }
 
    return success;
}
 
bool sdlMediaStartup(int dificulty){
    bool success;
	globalDificulty = dificulty;
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
	switch(dificulty){
		case easy:
			bgMusic = Mix_LoadMUS("Sound/musice.mp3");
			break;
		case normal:
			bgMusic = Mix_LoadMUS("Sound/musicm.mp3");
			break;
		case hard:
			bgMusic = Mix_LoadMUS("Sound/musich.mp3");
			break;
	}
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
 
void drawEggs(SDL_Renderer *renderer,Egg **matrix, int flag){
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
            
 			if(flag == 1)
 				radius = 100;

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
 
    buster->Pos.x = WINDOW_CENTER + relative_pos.x + EGG_TILE_SIZE/2 -13;
    buster->Pos.y = WINDOW_CENTER + relative_pos.y - EGG_TILE_SIZE + 13;
    buster->Pos.w = EGG_TILE_SIZE;
    buster->Pos.h = EGG_TILE_SIZE;
 
    SDL_RenderCopy(renderer, buster->busterTexture, NULL, &(buster->Pos));
	printf( "%s", score_char );
}
 
void moveBuster(){
	float x = 0, y = 0;
	int i, j;
	const Uint8 *press = SDL_GetKeyboardState(NULL);
	if( x != WINDOW_CENTER + relative_pos.x + EGG_TILE_SIZE/2 -13){
		buster->Pos.x = WINDOW_CENTER + relative_pos.x + EGG_TILE_SIZE/2 -13;
		buster->Pos.y = WINDOW_CENTER + relative_pos.y - EGG_TILE_SIZE + 13;
		buster->Pos.w = EGG_TILE_SIZE;
		buster->Pos.h = EGG_TILE_SIZE;
		move = true;
	}
    x = WINDOW_CENTER + relative_pos.x + EGG_TILE_SIZE/2 -13;
	y = WINDOW_CENTER + relative_pos.x + EGG_TILE_SIZE + 13;

	move = true;

	if(press[SDL_SCANCODE_P]){
		theHellisComing(globalDificulty);
        move = true;
	}
	if(press[SDL_SCANCODE_SPACE]){
		while( verifyCollide(buster,matrix) != true ){	
			buster->Pos.x += i * cos( angle_rad );
			buster->Pos.y += j * sin( angle_rad );
			i++; j++;
			if( buster->Pos.x > SCREEN_WIDTH || buster->Pos.y > SCREEN_HEIGHT ){
				break;
			}else if( buster->Pos.x < 0 || buster->Pos.y < 0 ){
				break;
			}
			move = true;
			if(move == true){
				reDraw(no_generated);
				move = false;
			}
		}
	}
}
 
 
void reDraw(int flag){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderCopy(renderer, buster->busterTexture, NULL, &(buster->Pos));
    drawEggs(renderer,matrix,flag);
	draw_arm( renderer );
	drawCurrentScore( renderer, score_char );
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
                    destroyEggs(i,j,buster->typeCode,buster->typeCode-4 );
					return true;
                }
            }

        }
    }
    return false;
}


//para dibujar el score!
void drawCurrentScore( SDL_Renderer* renderer, char* score_char ){
	SDL_Rect textPos_score = { 310, 500, 0, 0 };
	scoreTextSurface = TTF_RenderUTF8_Shaded( scoreDisplayFont, score_char, whitescore, blackscore );
	textPos_score.w = scoreTextSurface->w;
	textPos_score.h = scoreTextSurface->h;
	scoreTextureFont = SDL_CreateTextureFromSurface( renderer, scoreTextSurface );
	SDL_SetTextureBlendMode( scoreTextureFont, SDL_BLENDMODE_BLEND );
	SDL_SetTextureAlphaMod( scoreTextureFont, 99 );
	SDL_RenderCopy( renderer, scoreTextureFont, NULL, &textPos_score );
	SDL_FreeSurface( scoreTextSurface );
}

//termina de dibujar el Score!

void move_arm(){
	const Uint8 *press = SDL_GetKeyboardState(NULL);
	if(press[SDL_SCANCODE_LEFT]){
		update_angle(0, ANGLE_STEP);
		move = true;
	}
	if(press[SDL_SCANCODE_RIGHT]){
		update_angle(0, -ANGLE_STEP);
		move = true;
	}
	if(press[SDL_SCANCODE_UP]){
		update_angle(1, ANGLE_STEP);
		move = true;
	}
	if(press[SDL_SCANCODE_DOWN]){
		update_angle(1, -ANGLE_STEP);
		move = true;
	}
	if(press[SDL_SCANCODE_W]){
		update_angle(2, ANGLE_STEP);
		move = true;
	}
	if(press[SDL_SCANCODE_S]){
		update_angle(2, -ANGLE_STEP);
		move = true;
	}
    if(move == true){
        reDraw(generated);
	}
   	move = false;
}

void destroyEggs(int i, int j,int bustertype, int eggtype ){
	
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
				switch( globalDificulty ){
					case easy:
						score += 25;
						break;
					case normal:
						score += 50;
						break;
					case hard:
						score += 100;
						break;
				}
				sprintf( score_char, "%d", score );
				drawCurrentScore( renderer, score_char );
				buster = NULL;
				drawBuster();
			}

		}
	}
}

void theHellisComing(int dificulty){
	int i = 0, j = 0, eggsGenerated;
	float circleEQ;
	eggsGenerated = 0;
	int state = 0;

	printf("ENTRO A LA FUNCION!\n");

	while(eggsGenerated < (dificulty+2)){
		i = rand()%ROWS;
		j = rand()%COLUMNS;
		printf("BUCLE\n");
		printf("i:%d j:%d\n",i,j);
		if((matrix[i][j]).colorCode == nil){
			(matrix[i][j]).colorCode = (rand()%4)+1;
			printf("Se generó %d\n",(matrix[i][j]).colorCode);
			
			printf("HUEVOS Generados: %d\n",eggsGenerated);

            eggsGenerated++;
            state = 1;
            
            reDraw(generated);
		}
	}
	printf("DIBUJARA\n");
	if(state == 1){
		reDraw(generated);
	}
}
