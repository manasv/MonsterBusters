#include <stdio.h>
#include "highScore.c"

#define SCOREW 300
#define SCOREH 600
#define STATE1 0
#define STATE2 50
#define STATE3 100

//todas las variables terminan con S o con SC por SCORE.
//es solo para que cuando se unifiquen no halla conflicto

SDL_Window* scorew = NULL;
SDL_Surface* buttonScores = NULL;
SDL_Surface* bgSurfacesc = NULL;
SDL_Surface* imgSurfacesc = NULL;
SDL_Surface* winSurfacesc = NULL;
SDL_Rect buttonRectsc = { 0, 0, 100, 50 };
SDL_Rect buttonPossc = { 100, 450, 100, 50 };
SDL_Renderer* score_render = NULL;
SDL_Texture* buttonScoret = NULL;
SDL_Texture* bgTexturesc = NULL;
SDL_Event eventScore;


void Score_initWindow();
player* getHighscoreTable();
int buttonAction();
void renderScore();

int main( int argc, char** argv ){
	int terminate = 0;
	player* highScore = malloc( sizeof(player) * 10 ); //la tabla!
	highScore = getHighscoreTable();
	printTable( highScore );
	Score_initWindow();
	renderScore();
	while( !terminate ){
		if( SDL_WaitEvent( &eventScore ) ){
			if( eventScore.type == SDL_QUIT ){
				terminate = 1;
				SDL_DestroyWindow( scorew );
				IMG_Quit();
				SDL_Quit();
			}else{
				terminate = buttonAction();
			}
		}
	}
}

void Score_initWindow(){
	SDL_Init( SDL_INIT_EVERYTHING );
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	scorew = SDL_CreateWindow( 
		"HighScore",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCOREW,
		SCOREH,
		0
	);
	score_render = SDL_CreateRenderer( scorew, -1, SDL_RENDERER_ACCELERATED );
	SDL_SetRenderDrawColor( score_render, 0x20, 0x20, 0x20, 0xFF );
	winSurfacesc = SDL_GetWindowSurface( scorew );
	imgSurfacesc = IMG_Load( "./Img/background.jpg" );
	SDL_BlitSurface( imgSurfacesc, NULL, winSurfacesc, NULL );
	SDL_UpdateWindowSurface( scorew );
	bgTexturesc = SDL_CreateTextureFromSurface( score_render, imgSurfacesc );
	SDL_FreeSurface( imgSurfacesc );
	imgSurfacesc = IMG_Load( "./Img/OK.png" );
	buttonScoret = SDL_CreateTextureFromSurface( score_render, imgSurfacesc );
	SDL_FreeSurface( imgSurfacesc );
	renderScore();
}

//gets the highScore stats, or creates them if there's nothing

player* getHighscoreTable(){
	FILE* frb; FILE* fr;
	int i, aux; char* staux = malloc( 10 );

	player* highScore = malloc( sizeof( player ) * 10 );
	frb = fopen( "./Saves/scores.t", "rb" );
	fr = fopen( "./Saves/players.t", "r" );

	if( frb == NULL ){
		highScore = fillScores();
	}else{
		highScore = malloc( sizeof( player ) * 10 );
		for( i = 0; i < 10; i++ ){
			highScore[i].name = malloc( 10 );
			fscanf( fr, "%s", staux );
			strcpy( highScore[i].name, staux );
			fread( &highScore[i].score, sizeof(int), 1, frb);
		}
		fclose( frb );
		fclose( fr );
	}

	return highScore;
}

//button actions

int buttonAction(){
	int x = eventScore.button.x;
	int y = eventScore.button.y;
	int w = buttonPossc.w;
	int h = buttonPossc.h;
	int value = 0;
	if( x > buttonPossc.x && x < buttonPossc.x + w && y > buttonPossc.y && y < buttonPossc.y + h){
		buttonRectsc.y = STATE2;
		if( eventScore.button.button == SDL_BUTTON_LEFT && eventScore.button.state == SDL_RELEASED ){
			buttonRectsc.y = STATE3;
			value = 1;
		}
		renderScore();
		return value;
	}else{
		buttonRectsc.y = STATE1;
		value = 0;
		renderScore();
	}
	return value;
}

void renderScore(){
	SDL_RenderClear( score_render );
	SDL_RenderCopy( score_render, bgTexturesc, NULL, NULL );
	SDL_RenderCopy( score_render, buttonScoret, &buttonRectsc, &buttonPossc  );
	SDL_RenderPresent( score_render );
}
