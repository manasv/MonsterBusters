#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_video.h"

#define WINDOW_SIZE 800
#define WINDOW_CENTER_W (WINDOW_SIZE/2) //tomar en cuenta las dimensiones de la imagen!
#define WINDOW_CENTER_H (WINDOW_SIZE/2) //por lo tanto separar altura de ancho

#define CART_X(x) x + WINDOW_CENTER_W
#define CART_Y(y) WINDOW_SIZE - (WINDOW_CENTER_H + y)

//no entiendo por que -65 y -20 pero eso lo pone en el centro aparente
//quizas por que el eje esta en el centro de la imagen y no en el circulo
//de la punta! puede ser!

typedef struct {
	float cx;
	float cy;
} vector;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;
SDL_Surface *background = NULL;
SDL_Surface *axis = NULL;

SDL_Texture *background_tex = NULL;
SDL_Texture *arm_tex = NULL;
SDL_Texture *axis_tex = NULL;
SDL_Surface *arm_sur;


void draw(double angle) {
	SDL_Rect arm_rect  = {CART_X(axis->w/2), CART_Y(arm_sur->h/2), arm_sur->w, arm_sur->h};
	SDL_Point center   = {-axis->w/2, arm_sur->h/2}; //Cambio a respectiva definicion
	SDL_Rect axis_rect = {CART_X(-(axis->w/2)), CART_Y(axis->h/2), axis->w, axis->h};
	//Cambio a respectiva definicion
 	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_tex, NULL, NULL);
    SDL_RenderCopyEx(renderer, arm_tex, NULL, &arm_rect, (float)angle, &center, SDL_FLIP_NONE);
    SDL_RenderCopy(renderer, axis_tex, NULL, &axis_rect);
	/*Tenia NULL donde ahora tiene &d 
	 *Por eso lo renderizaba en la esquina, por que &r tiene posicion 0,0 ver linea 23<<
	 *No se para que es &r, no se usa, pero lo dejare ahi por si acaso!
	*/
	
	SDL_RenderPresent(renderer);
}

int main( int argc, char** argv ) { //cambie esto de main(void) a lo que esta ahorita
	int finish = 0;
	double angle = 0.0;
	SDL_Event evt;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, 0, &window, &renderer);
	screen_surface = SDL_GetWindowSurface(window);
	background = IMG_Load("./Img/background.jpg"); //la imagen se movio al directorio ./Img
	arm_sur    = IMG_Load("./Img/A.png"); //la imagen se movio al directorio ./Img
	axis       = IMG_Load("./Img/AXIS.png");
	printf("Axis: %d, %d\n", axis->w, axis->h);
	arm_tex = SDL_CreateTextureFromSurface(renderer, arm_sur);
	background_tex = SDL_CreateTextureFromSurface(renderer, background);
	axis_tex = SDL_CreateTextureFromSurface(renderer, axis);
	draw(angle);

	while(!finish) {
		if(SDL_WaitEvent(&evt)) {
			switch(evt.type) {
				case SDL_QUIT:
					finish = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(evt.button.button == SDL_BUTTON_LEFT) {
						angle = (atan((double)(evt.button.y-300)/(double)(evt.button.x-300)) * 180.0)/3.141592;
						if(evt.button.x-300 < 0) {
							angle = angle + 180.0;
						}
						draw(angle);
						printf("(%d, %d) = %.2f\n", evt.button.x, evt.button.y, angle);		
					}
					break;
				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_RIGHT) {
						angle += 5.0;
					}
					if(evt.key.keysym.sym == SDLK_LEFT) {
						angle -= 5.0;
					}
					draw(angle);
					break;
			}
		}
	}
	SDL_FreeSurface(background);
	SDL_Quit();
	return 0;
}
