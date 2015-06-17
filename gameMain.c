#include "Libs/mainLib.h"

int main(int argc, char const *argv[]){

	Egg **eggMatrix = NULL;
	bool active = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Monster Busters",SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 23, 0, 82, 1);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
	bgMusic = Mix_LoadMUS("Sound/music.mp3");

	allocateMatrix(&eggMatrix);
	fillMatrix(eggMatrix);
	background = IMG_LoadTexture(renderer,"Img/background.jpg");
	SDL_RenderCopy(renderer, background, NULL, NULL);
	drawEggs(renderer,eggMatrix);
	showMatrix(eggMatrix);
	Mix_PlayMusic(bgMusic,-1);

	while(active) {
		SDL_RenderPresent(renderer);
		while( SDL_PollEvent( &events ) != 0 ){
			if( events.type == SDL_QUIT ){
				active = false;
				freeEggs(eggMatrix,ROWS);
			}
		}
	}
	Mix_FreeMusic(bgMusic); 
	bgMusic = NULL;
	SDL_DestroyWindow(window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}