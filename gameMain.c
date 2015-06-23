#include "Libs/mainLib.h"

int main(int argc, char const *argv[]){

	bool active = true;
	
	sdlStartup(); //Startup SDL Subsystems
	sdlMediaStartup(); //Startup SDL Media Resources
	allocateMatrix(&matrix); //Allocate Memory Space For The Matrix
	fillMatrix(matrix); //Fill Matrix with identifiers for Egg Types
	drawEggs(renderer,matrix); //Draw Eggs on renderer
	showMatrix(matrix); //Show Matrix on Console, for testing purposes

	while(active){
		if( SDL_PollEvent( &events ) != 0 ){
			if( events.type == SDL_QUIT ){
				active = false;
			}
		}
		SDL_RenderPresent(renderer);
	}

	closeALL();
	return 0;
}