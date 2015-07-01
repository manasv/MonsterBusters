
SDL_Window* textBox;
SDL_Surface* boxSurface;
SDL_Surface* loadingSurface;
TTF_Font* playerFont = NULL;
SDL_Texture* playerFontTexture = NULL;
SDL_Renderer* inputRender;
SDL_Texture* textbox_bg;
SDL_Event keyPress;
SDL_Rect playerTextPos = { 70, 50, 0, 0 };
Sint32 cursor;
Sint32 selection_len;
SDL_Color white2 = { 255, 255, 255 };

void initWindows();
void inputActivate( char* playerName );
void updateInput( char* compose );

char* textboxGet(){
	char* playerName = malloc( 20 );
 	initWindows();
	inputActivate( playerName );
	return playerName;
}

//inicializando

void initWindows(){
	char* playerName = malloc( 20 );
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
	playerFont = TTF_OpenFont( "./Fonts/PragmataPro for Powerline.ttf", 21 );
	loadingSurface = TTF_RenderUTF8_Blended( playerFont, playerName, white2 );
	playerFontTexture = SDL_CreateTextureFromSurface( inputRender, loadingSurface );
	SDL_RenderPresent( inputRender );
}

void inputActivate( char* playerName ){
	char* compose = malloc( 20 );
	int done = 0, i;
	for( i = 0; i < 4; i++ ){
		compose[i] = ' ';
	}
 	SDL_StartTextInput();
	while( !done ){
		if( SDL_WaitEvent( &keyPress ) ){
			switch ( keyPress.type ){
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_TEXTINPUT:
					i++;
					sprintf( compose, "%s%s", compose, keyPress.text.text);
					updateInput( compose );
					printf("%s\n", compose);
				break;
				case SDL_KEYDOWN:
					if( keyPress.key.state == SDL_PRESSED && keyPress.key.keysym.sym == SDLK_RETURN 
						|| keyPress.key.state == SDL_PRESSED && keyPress.key.keysym.sym == SDLK_SPACE ){
						done = 1;
					}
					break;
				case SDL_TEXTEDITING:
				//	compose = keyPress.edit.text;
					cursor = keyPress.edit.start;
					selection_len = keyPress.edit.length;
					break;
			}
		}
	}
	sprintf( playerName, "%s", compose );
	SDL_DestroyWindow( textBox );
	IMG_Quit();
	SDL_Quit();
}

void updateInput( char* compose ){
	loadingSurface = TTF_RenderUTF8_Blended( playerFont, compose, white2 );
	playerFontTexture = SDL_CreateTextureFromSurface( inputRender, loadingSurface );
	playerTextPos.w = loadingSurface->w;
	playerTextPos.h = loadingSurface->h;
	SDL_RenderClear( inputRender );
	SDL_RenderCopy( inputRender, textbox_bg, NULL, NULL );
	SDL_RenderCopy( inputRender, playerFontTexture, NULL, &playerTextPos );
	SDL_RenderPresent( inputRender );
}
