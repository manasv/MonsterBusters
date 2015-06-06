#include "SDL2/SDL.h"

int main(int argc, char const *argv[]){
	int i;

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_INIT Error: ");
		SDL_GetError();
		printf("\n");
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == NULL){
		printf("SDL_CreateWindow Error: ");
		SDL_GetError();
		printf("\n");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		SDL_DestroyWindow(win);
		printf("SDL_CreateRenderer Error: ");
		SDL_GetError();
		printf("\n");
		SDL_Quit();
		return 1;
	}

	SDL_Surface *bmp = SDL_LoadBMP("Img/duke.bmp");
	if (bmp == NULL){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		printf("SDL_LoadBMP Error: ");
		SDL_GetError();
		printf("\n");
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == NULL){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		printf("SDL_CreateTextureFromSurface Error: ");
		SDL_GetError();
		printf("\n");
		SDL_Quit();
		return 1;
	}

	for (i = 0; i < 3; i++){
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
//at the end of this doc, deviantfero added a comment
//nuf said...
