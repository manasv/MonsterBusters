#include "Libs/mainLib.h"


int main(int argc, char const *argv[]){
 	
    bool isRunning = true;
    Uint32 start;

    sdlStartup(); //Startup SDL Subsystems
    sdlMediaStartup(); //Startup SDL Media Resources
    allocateMatrix(&matrix); //Allocate Memory Space For The Matrix
    fillMatrix(matrix); //Fill Matrix with identifiers for Egg Types
    drawEggs(renderer,matrix); //Draw Eggs on renderer
    drawBuster(); //Draw The Egg Buster on screen center
    showMatrix(matrix); //Show Matrix on Console, for testing purposes
 
    while(isRunning){
    	start = SDL_GetTicks();
        if( SDL_PollEvent(&events) != 0 ){
        	switch(events.type){

        		case SDL_QUIT:
        			isRunning = false;
        			break;

        		default:
        			break;
        	}
        	moveBuster(); 
        }

        //SDL_RenderPresent(renderer);
        if(TICKS_PER_FRAME > SDL_GetTicks() - start){
        	SDL_Delay(TICKS_PER_FRAME - (SDL_GetTicks() - start));
        }
        
    }
    closeALL();
    return 0;
}
