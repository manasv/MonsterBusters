#include "Libs/mainLib.h"

#define WINDOW_H 600
#define WINDOW_W 300

SDL_Window* menu_w1 = NULL;
SDL_Surface* bgSurface = NULL;
SDL_Surface* buttonSurface = NULL;
SDL_Texture* bgTexture = NULL;
SDL_Surface* wSurface;
SDL_Renderer* render = NULL;
SDL_Texture* buttonTexture = NULL;
SDL_Rect buttonRect = { 0, 0, 100, 50 }; //relativo a imagen
SDL_Rect buttonPos = { 100, 450, 100, 50 }; //relativo a ventana
SDL_Event mouseMove;
SDL_Event keyPress;
void drawMenu();

int main( int argc, char** argv ){
	int quit = 0;
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
	drawMenu();
	//Infinite loop event
	while( !quit ){
		if( SDL_WaitEvent( &mouseMove ) ){
			if( mouseMove.type == SDL_QUIT ){
				quit = 1;
			}
			//Mouse position and movement
			if( mouseMove.type == SDL_MOUSEMOTION ){
				if( mouseMove.motion.x > 100 && mouseMove.motion.x < 200 ){
					if( mouseMove.motion.y > 450 && mouseMove.motion.y < 500 ){
						buttonRect.y = 50;
						drawMenu();
						/*evaluate if there is a click once that hovering
						 *is taking place*/
						if( SDL_WaitEvent( &keyPress ) ){
							if( keyPress.button.button == SDL_BUTTON_LEFT ){
								if( keyPress.button.state == SDL_PRESSED ){
									buttonRect.y = 100;
									drawMenu();
									printf(" HUY\n");
								}
							}
						}
					}else{
						buttonRect.y = 0;
						drawMenu();
					}
			//mouseEvents
				}else{
					buttonRect.y = 0;
					drawMenu();
				}
			}
		}
	}
}

void drawMenu( void ){
	SDL_RenderClear( render );
	SDL_RenderCopy( render, bgTexture, NULL, NULL );
	SDL_RenderCopy( render, buttonTexture, &buttonRect,  &buttonPos);
	SDL_RenderPresent( render ); 
}
