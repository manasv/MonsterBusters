//#include <stdio.h> //only used when stand alone
#include "highScore.h" //only used when stand alone
//#include <SDL2/SDL_ttf.h> //only used when stand alone

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
SDL_Surface* playerSurface[10]; //this is where fonts will render.
SDL_Surface* scoreSurface[10];
SDL_Rect buttonRectsc = { 0, 0, 100, 50 };
SDL_Rect buttonPossc = { 100, 450, 100, 50 };
SDL_Renderer* score_render = NULL;
SDL_Texture* buttonScoret = NULL;
SDL_Texture* bgTexturesc = NULL;
SDL_Texture* playerTexture[10];
//for the ten players-name!
SDL_Texture* scoreTexture[10];
SDL_Event eventScore;
TTF_Font* scoreFont = NULL;
SDL_Color black = { 255, 255, 255 };
SDL_Rect playerPos = { 30, 100, 100, 100 };
SDL_Rect playerRect = { 0, 0, 100, 100 };
SDL_Rect scorePos = { 30, 100, 100, 100 };
SDL_Rect scoreRect = { 0, 0, 100, 100 };

player* highScore;

void Score_initWindow();
player* getHighscoreTable();
int buttonAction();
void renderScore();

int showScore(){
	int terminate = 0;
	highScore = malloc( sizeof( player ) * 10 );
	highScore = getHighscoreTable();
	printTable( highScore );
	Score_initWindow();
	renderScore();
	while( !terminate ){
		if( SDL_WaitEvent( &eventScore ) ){
			if( eventScore.type == SDL_QUIT ){
				terminate = 1;
			}else{
				terminate = buttonAction();
			}
		}
	}
	SDL_DestroyWindow( scorew );
}

void Score_initWindow(){
	int i;
	char* str[10];
	for( i = 0; i < 10; i++ ){
		str[i] = malloc( 15 );
	}
	SDL_Init( SDL_INIT_EVERYTHING );
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	TTF_Init();
	scorew = SDL_CreateWindow( 
		"HighScore",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCOREW,
		SCOREH,
		SDL_WINDOW_BORDERLESS
	);
	score_render = SDL_CreateRenderer( scorew, -1, SDL_RENDERER_ACCELERATED );
	scoreFont = TTF_OpenFont( "./Fonts/PragmataPro for Powerline.ttf", 21 );
	for( i = 0; i < 10; i++ ){
		playerSurface[i] = TTF_RenderUTF8_Blended( scoreFont, highScore[i].name, black );
		playerTexture[i] = SDL_CreateTextureFromSurface( score_render, playerSurface[i] );
		// cargando nombres del highscore!
	}
	for( i = 0; i < 10; i++ ){
		sprintf( str[i], "%d", highScore[i].score );
		scoreSurface[i] = TTF_RenderUTF8_Blended( scoreFont, str[i], black );
		scoreTexture[i] = SDL_CreateTextureFromSurface( score_render, scoreSurface[i] );
	}

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
		if( eventScore.button.button == SDL_BUTTON_LEFT && eventScore.button.state == SDL_PRESSED ){
			buttonRectsc.y = STATE3;
			value = 1;
		}
		eventScore.button.state = SDL_RELEASED;
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
	int i;
	SDL_RenderClear( score_render );
	SDL_RenderCopy( score_render, bgTexturesc, NULL, NULL );
	SDL_RenderCopy( score_render, buttonScoret, &buttonRectsc, &buttonPossc  );
	for( i = 0; i < 10; i++ ){
		playerRect.w = playerSurface[i]->w;
		playerRect.h = playerSurface[i]->h;
		playerPos.w = playerRect.w;
		playerPos.h = playerRect.h;
		playerPos.y = i*playerRect.h + 100; //mueve el siguiente texto! + separacion entre palabras
		scoreRect.w = scoreSurface[i]->w;
		scoreRect.h = scoreSurface[i]->h;
		scorePos.w = scoreRect.w;
		scorePos.h = scoreRect.h;
		scorePos.y = i*scoreRect.h + 100;
		scorePos.x = playerPos.x + 200; //separacion entre jugador y su score!
		SDL_RenderCopy( score_render, scoreTexture[i], &scoreRect, &scorePos );
		SDL_RenderCopy( score_render, playerTexture[i], &playerRect, &playerPos );
	}
	SDL_RenderPresent( score_render );
}
