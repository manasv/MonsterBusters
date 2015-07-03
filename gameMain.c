//#include "Libs/mainLib.h"

int game(int dificulty);

int game(int dificulty){

    bool isRunning = true;
    Uint32 start;

    sdlStartup(); //Startup SDL Subsystems
    sdlMediaStartup(dificulty); //Startup SDL Media Resources
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

        SDL_RenderPresent(renderer);
        if(TICKS_PER_FRAME > SDL_GetTicks() - start){
        	SDL_Delay(TICKS_PER_FRAME - (SDL_GetTicks() - start));
        }
        
    }
	score_gen( score );
//  closeALL();
	if( !game_over() ){
		closeALL();
	}else{
		SDL_DestroyWindow( window );
		score = 0; //reinicia el score a 0 para la proxima jugada
		Mix_HaltMusic();
		main();		
	}
	Mix_ResumeMusic();
    return 0;
}
