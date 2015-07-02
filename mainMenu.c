#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include "Libs/mainLib.h"
#include "Libs/libget.h"
#include "Libs/guiScore.h"
#include "gameMain.c"

#define WINDOW_H 500
#define WINDOW_W 700
#define STATE1 0
#define STATE2 50
#define STATE3 100

SDL_Window* menu_w1 = NULL;

SDL_Surface* bgSurface = NULL;
SDL_Surface* buttonSurface = NULL;
SDL_Texture* bgTexture = NULL;
SDL_Surface* wSurface;
SDL_Renderer* render = NULL;
SDL_Texture* buttonTexture = NULL;
SDL_Texture* buttonTexture2 = NULL;
SDL_Texture* buttonTexture3 = NULL;
SDL_Texture* buttonTexture4 = NULL;
SDL_Texture* buttonTexture5 = NULL;
SDL_Rect buttonRect = { 0, 0, 100, 50 }; //que pedazo de la imagen!
SDL_Rect buttonPos = { 220, 310, 100, 50 }; //relativo a ventana (posicion , tamaño)
SDL_Rect buttonRect2 = { 0, 0, 50, 50 };
SDL_Rect buttonPos2 = { 320, 260, 50, 50 };
SDL_Rect buttonRect3 = { 0, 0, 50, 50 };
SDL_Rect buttonPos3 = { 375, 260, 50, 50 };
SDL_Rect buttonRect4 = { 0, 0, 50, 50 };
SDL_Rect buttonPos4 = { 429, 260, 50, 50 };
SDL_Rect buttonRect5 = { 0, 0, 100, 50 };
SDL_Rect buttonPos5 = { 390, 310, 100, 50 };

SDL_Event mEvnt;
SDL_Event keyPress;

Mix_Chunk* beep = NULL;



void drawMenu();
void initMenu();


int main( int argc, char** argv ){
	int quit = 0;
	int value = 0, flag[3] = { 0, 0, 0 };
	int x, y, w, h, w2, h2, w3, h3, w4, h4, w5, h5;
	w = buttonPos.w; //for later ease of use
	h = buttonPos.h;
	w2 = buttonPos2.w;
	h2 = buttonPos2.h;
	w3 = buttonPos3.w;
	h3 = buttonPos3.h;
	w4 = buttonPos4.w;
	h4 = buttonPos4.h;
	w5 = buttonPos5.w;
	h5 = buttonPos5.h;
	//initializing SDL
	if( SDL_Init(SDL_INIT_EVERYTHING) ){
		printf( "SDL Sucked Balls!\n" );
		exit(1);
	}

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		exit(0);
	}

	beep = Mix_LoadWAV( "Sound/select.wav" );
	//image rect details
	initMenu();
	//Infinite loop event
	//el detalle de los botones era evaluar la poscion
	//de event.button.x y event.button.y, no event.motion.x...
	while( !quit ){
		if( SDL_WaitEvent( &mEvnt ) ){ //Do_while
			if( mEvnt.type == SDL_QUIT ){
				quit = 1;
			}
			x = mEvnt.button.x; 
			y = mEvnt.button.y;
		} // instead of another event, I just use SDL_GetMouseState and SDL_button!
		if( x > buttonPos.x && x < buttonPos.x + w && y > buttonPos.y && y < buttonPos.y + h){
			buttonRect.y = STATE2;
			if( mEvnt.button.button == SDL_BUTTON_LEFT && mEvnt.button.state == SDL_PRESSED ){
				buttonRect.y = STATE3;
				printf(" START\n");
				Mix_PlayChannel( -1, beep, 0 );
				if( flag[0] == 1 ){
					value = 1;
					printf("difficulty -- %d\n", value);
					SDL_DestroyWindow( menu_w1 );
					game( value );
					break;
				}else if( flag[1] == 1 ){
					value = 2;
					SDL_DestroyWindow( menu_w1 );
					game( value );
					break;
				}else if( flag[2] == 1 ){
					value = 3;
					SDL_DestroyWindow( menu_w1 );
					game( value );
					break;
				}else{
					printf("No difficulty was selected!\n");
				}
			}
			drawMenu();
		}else{
			buttonRect.y = STATE1;
			drawMenu();
		}
		if( x > buttonPos2.x && x < buttonPos2.x + w2 && y > buttonPos2.y && y < buttonPos2.y + h2 ){
			buttonRect2.y = STATE2;
			if( mEvnt.button.button == SDL_BUTTON_LEFT && mEvnt.button.state == SDL_PRESSED ){
				buttonRect2.y = STATE3;
				printf("EASY\n");
				Mix_PlayChannel( -1, beep, 0 );
				flag[0] =  1; flag[1] = 0; flag[2] = 0;
			}
			drawMenu();
		}else if( !flag[0] ){
			buttonRect2.y = STATE1;
			drawMenu();
		}
		if( x > buttonPos3.x && x < buttonPos3.x + w3 && y > buttonPos3.y && y < buttonPos3.y + h3 ){
			buttonRect3.y = STATE2;
			if(  mEvnt.button.button == SDL_BUTTON_LEFT && mEvnt.button.state == SDL_PRESSED ){
					buttonRect3.y = STATE3;
					printf( "MEDIUM\n" );
					Mix_PlayChannel( -1, beep, 0 );
					flag[0] = 0; flag[1] = 1; flag[2] = 0;
			}
			drawMenu();
		}else if( !flag[1] ){
			buttonRect3.y = STATE1;
			drawMenu();
		}
		if( x > buttonPos4.x && x < buttonPos4.x + w4 && y > buttonPos4.y && y < buttonPos4.y + h4 ){
			buttonRect4.y = STATE2;
			if(  mEvnt.button.button == SDL_BUTTON_LEFT && mEvnt.button.state == SDL_PRESSED ){
				buttonRect4.y = STATE3;
				printf( "HARD\n" );
				Mix_PlayChannel( -1, beep, 0 );
				flag[0] = 0; flag[1] = 0; flag[2] =1;
			}
			drawMenu();
		}else if( !flag[2] ){
			buttonRect4.y = STATE1;
			drawMenu();
		}
		if( x > buttonPos5.x && x < buttonPos5.x + w5 && y > buttonPos5.y && y < buttonPos5.y + h5 ){
			buttonRect5.y = STATE2;
			if( mEvnt.button.button == SDL_BUTTON_LEFT && mEvnt.button.state == SDL_PRESSED ){
				buttonRect5.y = STATE3;
				printf( "SCORES\n" );
				Mix_PlayChannel( -1, beep, 0 );
				showScore();
			}
			drawMenu();
		}else{
			buttonRect5.y = STATE1;
			drawMenu();
		} // añadir más botones aqui si es necesario!
	}
}

void initMenu(){
	
	menu_w1 = SDL_CreateWindow(
		"Menu",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_W,
		WINDOW_H,
		0
	);

	render = SDL_CreateRenderer( menu_w1, -1, SDL_RENDERER_ACCELERATED );	
	SDL_SetRenderDrawColor( render, 0x20, 0x20, 0x20, 0xFF );
	wSurface = SDL_GetWindowSurface( menu_w1 );
	bgSurface = IMG_Load( "./Img/OP.jpg" );
	SDL_BlitSurface( bgSurface, NULL, wSurface, NULL );
 	SDL_UpdateWindowSurface( menu_w1 );
	bgTexture = SDL_CreateTextureFromSurface( render, bgSurface );
	SDL_FreeSurface( bgSurface );
	buttonSurface = IMG_Load( "./Img/START.png" );
 	buttonTexture = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/EASY.png" );
	buttonTexture2 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/NORMAL.png" );
	buttonTexture3 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/HARD.png" );
	buttonTexture4 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/SCORE.png" );
	buttonTexture5 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	drawMenu();
}

void drawMenu( void ){
	SDL_RenderClear( render );
	SDL_RenderCopy( render, bgTexture, NULL, NULL );
	SDL_RenderCopy( render, buttonTexture, &buttonRect,  &buttonPos);
	SDL_RenderCopy( render, buttonTexture2, &buttonRect2, &buttonPos2 );
	SDL_RenderCopy( render, buttonTexture3, &buttonRect3, &buttonPos3 );
	SDL_RenderCopy( render, buttonTexture4, &buttonRect4, &buttonPos4 );
	SDL_RenderCopy( render, buttonTexture5, &buttonRect5, &buttonPos5 );
	SDL_RenderPresent( render ); 
}
