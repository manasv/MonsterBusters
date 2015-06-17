#include "Libs/mainLib.h"

int main(int argc, char const *argv[]){

	Egg **eggMatrix = NULL;
	bool active = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Monster Busters",SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
	IMG_Init(IMG_INIT_PNG);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	//SDL_SetRenderDrawColor(renderer, 23, 0, 82, 1);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	allocateMatrix(&eggMatrix);
	fillMatrix(eggMatrix);
	showMatrix(eggMatrix);
	background = IMG_LoadTexture(renderer,"Img/background.png");
	SDL_RenderCopy(renderer, background, NULL, NULL);
	drawEggs(renderer,eggMatrix);

	while(active) {
		SDL_RenderPresent(renderer);
		while( SDL_PollEvent( &events ) != 0 ){
			if( events.type == SDL_QUIT ){
				active = false;
				freeEggs(eggMatrix,ROWS);
			}
		}
	}

	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}