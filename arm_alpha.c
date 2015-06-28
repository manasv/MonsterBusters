#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_video.h"

#define WINDOW_SIZE 600
#define WINDOW_CENTER_W 300-65 //tomar en cuenta las dimensiones de la imagen!
#define WINDOW_CENTER_H 300-20 //por lo tanto separar altura de ancho

//no entiendo por que -65 y -20 pero eso lo pone en el centro aparente
//quizas por que el eje esta en el centro de la imagen y no en el circulo
//de la punta! puede ser!

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;
SDL_Surface *background = NULL;

SDL_Texture *background_tex = NULL;
SDL_Texture *arm_tex;
SDL_Surface *arm_sur;

SDL_Point center = {WINDOW_CENTER_W, WINDOW_CENTER_H}; //Cambio a respectiva definicion

void draw(int x, int y, double angle) {
	SDL_Rect r = {0, 0, 152, 53}; //Aqui las cordenadas de posicion estan en 0,0
	SDL_Rect d = {WINDOW_CENTER_W, WINDOW_CENTER_H, 152, 53};
	//Cambio a respectiva definicion
 	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_tex, NULL, NULL);
    SDL_RenderCopyEx(renderer, arm_tex, NULL, &d, (float)angle, NULL, SDL_FLIP_NONE);
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
	arm_sur = IMG_Load("./Img/arm.png"); //la imagen se movio al directorio ./Img
	arm_tex = SDL_CreateTextureFromSurface(renderer, arm_sur);
	background_tex = SDL_CreateTextureFromSurface(renderer, background);
	SDL_BlitSurface(background, NULL, screen_surface, NULL);
	SDL_UpdateWindowSurface(window);

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
						draw(evt.button.x, evt.button.y, angle);
						printf("(%d, %d) = %.2f\n", evt.button.x, evt.button.y, angle);		
					}
					break;
				
			}
		}
	}
	SDL_FreeSurface(background);
	SDL_Quit();
	return 0;
}
