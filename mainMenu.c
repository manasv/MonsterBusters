#include "Libs/mainLib.h"

#define WINDOW_H 600
#define WINDOW_W 300
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
SDL_Rect buttonRect = { 0, 0, 100, 50 }; //que pedazo de la imagen!
SDL_Rect buttonPos = { 100, 450, 100, 50 }; //relativo a ventana (posicion , tamaño)
SDL_Rect buttonRect2 = { 0, 0, 50, 50 };
SDL_Rect buttonPos2 = { 40, 60, 50, 50 };
SDL_Rect buttonRect3 = { 0, 0, 50, 50 };
SDL_Rect buttonPos3 = { 110, 60, 50, 50 };

SDL_Event mEvnt;
SDL_Event keyPress;
void drawMenu();


int main( int argc, char** argv ){
	int quit = 0;
	float x, y, w, h, w2, h2;
	w = buttonPos.w; //for later ease of use
	h = buttonPos.h;
	w2 = buttonPos2.w;
	h2 = buttonPos2.h;
	//initializing SDL
	if( SDL_Init(SDL_INIT_VIDEO) ){
		printf( "SDL Sucked Balls!\n" );
		exit(1);
	}


	menu_w1 = SDL_CreateWindow(
	"Menu",
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
	WINDOW_W,
	WINDOW_H,
	SDL_WINDOW_BORDERLESS 
	);

	//image rect details
	render = SDL_CreateRenderer( menu_w1, -1, SDL_RENDERER_ACCELERATED );	
	SDL_SetRenderDrawColor( render, 0x20, 0x20, 0x20, 0xFF );
	wSurface = SDL_GetWindowSurface( menu_w1 );
	bgSurface = IMG_Load( "./Img/background.jpg" );
	SDL_BlitSurface( bgSurface, NULL, wSurface, NULL );
 	SDL_UpdateWindowSurface( menu_w1 );
	bgTexture = SDL_CreateTextureFromSurface( render, bgSurface );
	SDL_FreeSurface( bgSurface );
	buttonSurface = IMG_Load( "./Img/button01.png" );
 	buttonTexture = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/button02.png" );
	buttonTexture2 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	buttonSurface = IMG_Load( "./Img/button03.png" );
	buttonTexture3 = SDL_CreateTextureFromSurface( render, buttonSurface );
	SDL_FreeSurface( buttonSurface );
	drawMenu();

	//Infinite loop event
	while( !quit ){
		if( SDL_PollEvent( &mEvnt ) ){ //Do_while
			if( mEvnt.type == SDL_QUIT ){
				quit = 1;
			}
			x = mEvnt.motion.x; 
			y = mEvnt.motion.y; 

			if( x > buttonPos.x && x < buttonPos.x + w && y > buttonPos.y && y < buttonPos.y + h){
				buttonRect.y = STATE2;
				drawMenu();
					if( SDL_WaitEvent( &keyPress ) ){
					if( keyPress.button.state == SDL_PRESSED && 
					keyPress.button.button == SDL_BUTTON_LEFT){
						buttonRect.y = STATE3;
						drawMenu();
						printf(" HUY\n");
					}
				}
			}else{
				buttonRect.y = STATE1;
				drawMenu();
			}
			if( x > buttonPos2.x && x < buttonPos2.x + w2 && y > buttonPos2.y && y < buttonPos2.y + h2 ){
				buttonRect2.y = STATE2;
				drawMenu();
				if( SDL_WaitEvent( &keyPress ) ){
					if( keyPress.button.state == SDL_PRESSED &&
					keyPress.button.button == SDL_BUTTON_LEFT){
						buttonRect2.y = STATE3;
						drawMenu();
						printf("SHIET\n");
					}
				}
			}else{
				buttonRect2.y = STATE1;
				drawMenu();
			}
			//añadir otros botones aqui!
		}
	}
}

void drawMenu( void ){
	SDL_RenderClear( render );
	SDL_RenderCopy( render, bgTexture, NULL, NULL );
	SDL_RenderCopy( render, buttonTexture, &buttonRect,  &buttonPos);
	SDL_RenderCopy( render, buttonTexture, &buttonRect2, &buttonPos2 );
	SDL_RenderCopy( render, buttonTexture, &buttonRect3, &buttonPos3 );
	SDL_RenderPresent( render ); 
}
