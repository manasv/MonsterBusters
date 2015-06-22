#include "Libs/mainLib.h"

#define WINDOW_H 300
#define WINDOW_W 600
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
SDL_Rect buttonRect = { 0, 0, 100, 50 }; //que pedazo de la imagen!
SDL_Rect buttonPos = { 100, 250, 100, 50 }; //relativo a ventana (posicion , tamaño)
SDL_Rect buttonRect2 = { 0, 0, 50, 50 };
SDL_Rect buttonPos2 = { 30, 60, 50, 50 };
SDL_Rect buttonRect3 = { 0, 0, 50, 50 };
SDL_Rect buttonPos3 = { 125, 60, 50, 50 };
SDL_Rect buttonRect4 = { 0, 0, 50, 50 };
SDL_Rect buttonPos4 = { 220, 60, 50, 50 };

SDL_Event mEvnt;
SDL_Event keyPress;
void drawMenu();


int main( int argc, char** argv ){
	int quit = 0;
	int dificulty = 0, flag[3] = { 0, 0, 0 };
	float x, y, w, h, w2, h2, w3, h3, w4, h4;
	w = buttonPos.w; //for later ease of use
	h = buttonPos.h;
	w2 = buttonPos2.w;
	h2 = buttonPos2.h;
	w3 = buttonPos3.w;
	h3 = buttonPos3.h;
	w4 = buttonPos4.w;
	h4 = buttonPos4.h;
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
	bgSurface = IMG_Load( "./Img/MONSTER_BUSTER.jpg" );
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
	buttonSurface = IMG_Load( "./Img/button04.png" );
	buttonTexture4 = SDL_CreateTextureFromSurface( render, buttonSurface );
	drawMenu();

	//Infinite loop event
	while( !quit ){
		if( SDL_PollEvent( &mEvnt ) ){ //Do_while
			if( mEvnt.type == SDL_QUIT ){
				quit = 1;
			}
		x = mEvnt.motion.x; 
		y = mEvnt.motion.y;
		//SDL_EventState( SDL_MOUSEMOTION, SDL_IGNORE );
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
						flag[0] =  1; flag[1] = 0; flag[2] = 0;
					}
				}
			}else if( !flag[0] ){
				buttonRect2.y = STATE1;
				drawMenu();
			}
			if( x > buttonPos3.x && x < buttonPos3.x + w3 && y > buttonPos3.y && y < buttonPos3.y + h3 ){
				buttonRect3.y = STATE2;
				drawMenu();
				if( SDL_WaitEvent( &keyPress ) ){
					if( keyPress.button.state = SDL_PRESSED &&
						keyPress.button.button == SDL_BUTTON_LEFT ){
							buttonRect3.y = STATE3;
							drawMenu();
							printf( "DAYUM\n" );
							flag[0] = 0; flag[1] = 1; flag[2] = 0;
					}
				}
			}else if( !flag[1] ){
				buttonRect3.y = STATE1;
				drawMenu();
			}
			if( x > buttonPos4.x && x < buttonPos4.x + w4 && y > buttonPos4.y && y < buttonPos4.y + h4 ){
				buttonRect4.y = STATE2;
				drawMenu();
				if( SDL_WaitEvent( &keyPress ) ){
					if( keyPress.button.state == SDL_PRESSED &&
						keyPress.button.button == SDL_BUTTON_LEFT ){
							buttonRect4.y = STATE3;
							drawMenu();
							printf( "FRIED CHICKEN NIGGA\n" );
							flag[0] = 0; flag[1] = 0; flag[2] =1;
					}
				}
			}else if( !flag[2] ){
				buttonRect4.y = STATE1;
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
	SDL_RenderCopy( render, buttonTexture, &buttonRect4, &buttonPos4 );
	SDL_RenderPresent( render ); 
}
