#include <stdio.h> //only used when stand alone
#include <stdlib.h> //only used when stand alone
#include <string.h> //only used when stand alone
#include "Libs/mainLib.h" //only used when stand alone
#include <SDL2/SDL_ttf.h>
#include "Libs/libget.h" // getint getstr getfloat only used when stand alone
#include "Libs/textinput.h"

SDL_Event keyPress;
SDL_Window* w1 = NULL;
Mix_Chunk* beepy = NULL;
SDL_Surface* window_surface = NULL;
SDL_Surface* loading_surface = NULL;
SDL_Surface* text_surface = NULL;
SDL_Texture* bg_texture = NULL;
SDL_Texture* coin = NULL;
SDL_Texture* text_texture = NULL;
SDL_Rect coinRect = { 0, 0, 128, 128 };
SDL_Rect coinPos = { 250, 250, 128, 128 };
SDL_Rect textRect = { 0, 0, 0, 0 };
SDL_Rect textPos = { 100, 100, 0, 0 };
SDL_Renderer* score_render;


TTF_Font* score_font = NULL;
SDL_Color white = { 255, 255, 255 };

typedef struct{
	char* name;
	int score;
}player;

FILE* fr; FILE* fw; //for reading and writing
FILE* frb; FILE* fwb; //for reading and writing binary

int getScore();
player* fillScores();
void printTable( player* prntTable );
player* sortHighScore( player player1, player* highScore );
void init_systems();
void render_screen();


//cambiar esto a main si se necesita generar un highscore
//antes de implementarlo en el juego!

int main( int argc, char** argv ){
	int i = 9, aux;
	char* staux = malloc( 10 );
	player player1;
	player* highScore;

	init_systems(); //inicializa el fondo basicamente

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
	highScore = sortHighScore( player1, highScore );
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

//Score functions that WILL be used on guiScore.c

int getScore(){
	char* scorestr = malloc( 15 );
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1){
		exit(0);
	}
	beepy = Mix_LoadWAV( "Sound/points.wav" );
	player p1 = { "aa", 0 };
	int quit = 0;
	score_font = TTF_OpenFont( "./Fonts/PragmataPro for Powerline.ttf", 21 );
	while( !quit ){
		if( SDL_WaitEvent( &keyPress ) ){
			if( keyPress.type == SDL_QUIT ){
				SDL_Quit();
				quit = 1;
			}
			if( keyPress.type == SDL_KEYDOWN ){
				if( keyPress.key.keysym.scancode == SDL_SCANCODE_SPACE ){
					p1.score += 100;
					sprintf( scorestr ,"%d", p1.score );
					text_surface = TTF_RenderUTF8_Solid( score_font, scorestr, white );
					textRect.w = text_surface->w;
					textRect.h = text_surface->h;
					textPos.w = text_surface->w;
					textPos.h = text_surface->h;
					text_texture = SDL_CreateTextureFromSurface( score_render, text_surface );
					SDL_FreeSurface( text_surface );
					render_screen();
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

//To fill Scores

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

//to print them on console

void printTable( player* prntTable ){
	int i; char* aux;
	for( i = 0; i  < 10; i++ ){
		printf("%s -- score: %d\n", prntTable[i].name, prntTable[i].score);
	}
}

//to Sort them in order
player* sortHighScore( player player1, player* highScore ){
	int i, aux;
	char* staux;
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
		highScore[i].name = textboxGet();
	}
	//returns sorted table
	return highScore;	
}

void init_systems(){
	SDL_Init( SDL_INIT_EVERYTHING);
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	TTF_Init();
	score_font = TTF_OpenFont( "Fonts/Pragmatapro for Powerline.ttf", 18 );
	w1 = SDL_CreateWindow( "Score", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, 0 );
	score_render = SDL_CreateRenderer( w1, -1, SDL_RENDERER_ACCELERATED );
	window_surface = SDL_GetWindowSurface( w1 );
	loading_surface = IMG_Load( "Img/background.jpg" );
	SDL_BlitSurface( loading_surface, NULL, window_surface, NULL );
	SDL_UpdateWindowSurface( w1 );
	SDL_FreeSurface( loading_surface );
	bg_texture = SDL_CreateTextureFromSurface( score_render, window_surface );
	loading_surface = IMG_Load( "Img/coin.png" );
	coin = SDL_CreateTextureFromSurface( score_render, loading_surface );
	SDL_FreeSurface( loading_surface );
	SDL_RenderClear( score_render );
	SDL_RenderCopy( score_render, bg_texture, NULL, NULL );
	SDL_RenderCopy( score_render, coin, &coinRect, &coinPos );
	SDL_RenderPresent( score_render );
}

void render_screen(){
	int i, j = 0;
	for( i = 0; i < 40; i++ ){
		if( i < 20 ){
			coinPos.y -= i;
			SDL_RenderClear( score_render );
			SDL_RenderCopy( score_render, bg_texture, NULL, NULL );
			SDL_RenderCopy( score_render, text_texture, &textRect, &textPos );
			SDL_RenderCopy( score_render, coin, &coinRect, &coinPos );
			SDL_RenderPresent( score_render );
			SDL_Delay( 5 );
		}else{
			coinPos.y += j;
			SDL_RenderClear( score_render );
			SDL_RenderCopy( score_render, bg_texture, NULL, NULL );
			SDL_RenderCopy( score_render, text_texture, &textRect, &textPos );
			SDL_RenderCopy( score_render, coin, &coinRect, &coinPos );
			SDL_RenderPresent( score_render );
			SDL_Delay( 5 );
			j++;
		}
	}
}

