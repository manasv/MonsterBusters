//#include "Libs/mainLib.h"

int game(int dificulty);

int game(int dificulty){

    bool isRunning = true;

    sdlStartup(); //Startup SDL Subsystems
    sdlMediaStartup(dificulty); //Startup SDL Media Resources
    allocateMatrix(&matrix); //Allocate Memory Space For The Matrix
    fillMatrix(matrix); //Fill Matrix with identifiers for Egg Types
	startEggTextures( renderer );
    showMatrix(matrix); //Show Matrix on Console, for testing purposes
	init_matrix();
	load_images( renderer );
	drawBuster(); //Draw The Egg Buster on screen center
//	move_arm();
//	drawBuster();

    currentTime = SDL_GetTicks();
 
    while(isRunning){
    	start = SDL_GetTicks();
        oldTime = currentTime;
        currentTime = SDL_GetTicks();

        if( SDL_PollEvent(&events) != 0 ){
        	switch(events.type){

        		case SDL_QUIT:
        			isRunning = false;
        			break;

        		default:
        			break;
        	}

    		drawEggs(renderer,matrix,no_generated); //Draw Eggs on renderer
			if(!move_arm()){
				isRunning = false;
			}
			moveBuster();
		}

        SDL_RenderPresent(renderer);
        if(TICKS_PER_FRAME > SDL_GetTicks() - start){
        	SDL_Delay(TICKS_PER_FRAME - (SDL_GetTicks() - start));
        }

        if((currentTime/1000)%30 == 0){
			theHellisComing(globalDificulty);
        }
        
    }
	score_gen( score );
//  closeALL();
	if( !game_over() ){
	//	closeALL(); esto cause el core dumped bizarro 
	//	con la generacion activada
		SDL_DestroyWindow( window );
	}else{
		SDL_DestroyWindow( window );
		score = 0; //reinicia el score a 0 para la proxima jugada
		Mix_HaltMusic();
		main();		
	}
	Mix_ResumeMusic();
    return 0;
}
