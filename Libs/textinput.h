
SDL_Window* textBox;
SDL_Surface* boxSurface;
SDL_Surface* loadingSurface;
SDL_Renderer* inputRender;
SDL_Texture* textbox_bg;

void initWindows();

char* textboxGet(){
	char* playerName = "macho";
 	initWindows();
	return playerName;
}

void initWindows(){

	SDL_Init( SDL_INIT_EVERYTHING );
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	TTF_Init();
	textBox = SDL_CreateWindow(
		"Highscore!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		300,
		100,
		SDL_WINDOW_BORDERLESS
	);
	inputRender = SDL_CreateRenderer( textBox, -1, SDL_RENDERER_ACCELERATED );
	boxSurface = SDL_GetWindowSurface( textBox );
	loadingSurface = IMG_Load( "./Img/background.jpg" );
	SDL_BlitSurface( loadingSurface, NULL, boxSurface, NULL );
	SDL_UpdateWindowSurface( textBox );
	textbox_bg = SDL_CreateTextureFromSurface( inputRender, loadingSurface);
	SDL_FreeSurface( loadingSurface );
	SDL_RenderClear( inputRender );
	SDL_RenderCopy( inputRender, textbox_bg, NULL, NULL );
	SDL_RenderPresent( inputRender );
	SDL_Delay( 1000 );

}
