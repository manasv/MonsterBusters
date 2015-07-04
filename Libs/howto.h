
SDL_Window* howto = NULL;
SDL_Texture* bg_howto = NULL;
SDL_Renderer* render_howto = NULL;
SDL_Surface* bghowto_surface = NULL;
SDL_Surface* loadhowto_surface = NULL;
SDL_Event howtoEvent;

void init_howto();

void Howto(){
	int clear = 0;
	init_howto();
	while( !clear ){
		if( SDL_WaitEvent( &howtoEvent ) ){
			if( howtoEvent.type == SDL_QUIT ){
				SDL_DestroyWindow( howto );
				clear = 1;
			}
			if( howtoEvent.key.keysym.sym == SDLK_ESCAPE || 
				howtoEvent.key.keysym.sym == SDLK_RETURN ||
				howtoEvent.key.keysym.sym == SDLK_SPACE
				&& howtoEvent.key.state == SDL_PRESSED ){
				SDL_DestroyWindow( howto );
				clear = 1;
			}
		}
	}
}

void init_howto(){
	SDL_Init( SDL_INIT_EVERYTHING );
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	howto = SDL_CreateWindow( 
		"How to Play!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		700,
		500,
		SDL_WINDOW_BORDERLESS
	);
	render_howto = SDL_CreateRenderer( howto, -1, SDL_RENDERER_ACCELERATED );
	bghowto_surface = SDL_GetWindowSurface( howto );
	loadhowto_surface = IMG_Load( "Img/HOW TO PLAY.jpg" );
	SDL_BlitSurface( loadhowto_surface, NULL, bghowto_surface, NULL );
	SDL_UpdateWindowSurface( howto );
	bg_howto = IMG_LoadTexture( render_howto, "Img/HOW TO PLAY.jpg" );
	SDL_RenderClear( render_howto );
	SDL_RenderCopy( render_howto, bg_howto, NULL, NULL );
	SDL_RenderPresent( render_howto );
}
