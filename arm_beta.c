#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_video.h"

#define WINDOW_SIZE 600
#define WINDOW_CENTER 300

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen_surface = NULL;
SDL_Surface *background = NULL;

SDL_Texture *background_tex = NULL;
SDL_Texture *arm_tex;
SDL_Surface *arm_sur;

SDL_Point center = {WINDOW_CENTER, WINDOW_CENTER};

void draw(int x, int y, double angle) {
	SDL_Rect r = {0, 0, 378, 132};
	SDL_Rect d = {WINDOW_CENTER, WINDOW_CENTER, 378, 132};

 	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_tex, NULL, NULL);
    SDL_RenderCopyEx(renderer, arm_tex, NULL, NULL, (float)angle, &center, SDL_FLIP_NONE);
	
	SDL_RenderPresent(renderer);
}

int main(void) {
	int finish = 0;
	double angle = 0.0;
	SDL_Event evt;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, 0, &window, &renderer);
	screen_surface = SDL_GetWindowSurface(window);
	background = IMG_Load("background.jpg");
	arm_sur = IMG_Load("arm.png");
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