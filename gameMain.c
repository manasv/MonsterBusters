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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
	bgMusic = Mix_LoadMUS("Sound/music.mp3");



	while(active) {
		SDL_RenderPresent(renderer);
		if( SDL_PollEvent( &events ) != 0 ){
			if( events.type == SDL_QUIT ){
				active = false;
				freeEggs(eggMatrix,ROWS);
			}
		}

		const Uint8* press = SDL_GetKeyboardState(NULL);
		if (press[SDL_SCANCODE_F1]) {
			SDL_SetWindowFullscreen(window,0);
		}
		else if(press[SDL_SCANCODE_F2]){
			SDL_SetWindowFullscreen(window,1);     
		}
		else if (press[SDL_SCANCODE_S]||press[SDL_SCANCODE_DOWN]) {
			cubePos.y-=4; 
		}else if(press[SDL_SCANCODE_A]||press[SDL_SCANCODE_LEFT]){
			cubePos.x-=4;            
		}else if(press[SDL_SCANCODE_D]||press[SDL_SCANCODE_RIGHT]){
			cubePos.x+=4;                
		}else if(press[SDL_SCANCODE_W]||press[SDL_SCANCODE_UP]) {
			cubePos.y+=4;  
		}

		reDraw();

	}
	   

	Mix_FreeMusic(bgMusic); 
	bgMusic = NULL;
	SDL_DestroyWindow(window);
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;

}


