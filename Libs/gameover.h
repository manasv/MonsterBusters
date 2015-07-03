#define STATE1 0
#define STATE2 50
#define STATE3 100
// no includes, sera incluido por mainMenu.c o, gameMain.c
//uso: solo llamar la funcion cuando el juego se termine
//la funcion se encargara del resto!


SDL_Texture* okButton = NULL;
SDL_Texture* retryButton = NULL;
SDL_Texture* gameoverBg = NULL;
SDL_Renderer* go_render = NULL; //gameover_render
SDL_Window* wgameOver = NULL;
SDL_Event mEvent;
SDL_Rect okRect = {0, 0, 100, 50};
SDL_Rect okPos = { 263, 192, 100, 50 };
SDL_Rect retryRect = {0, 0, 100, 50};
SDL_Rect retryPos = { 137, 192, 100, 50 };


void init_go();
void renderAll();

int game_over(){
	int leave = 0;
	int x, y;
	int w_go, h_go, w1_go, h1_go;
	w_go = okRect.w;
	h_go = okRect.h;
	init_go();
	while( !leave ){
		if( SDL_WaitEvent( &mEvent ) ){
			x = mEvent.button.x;
			y = mEvent.button.y;
			if( x > okPos.x && x < okPos.x + w_go && y > okPos.y && y < okPos.y + h_go ){
				okRect.y = STATE2;
				if( mEvent.button.button == SDL_BUTTON_LEFT && mEvent.button.state == SDL_PRESSED ){
					okRect.y = STATE3;
					printf("Salu\n");
					renderAll();
					SDL_DestroyWindow( wgameOver );
					return 0;
					leave = 1;
				}
				renderAll();
			}else{
				okRect.y = STATE1;
				renderAll();
			}
			if( x > retryPos.x && x < retryPos.x + w_go && y > retryPos.y && y < retryPos.y + h_go ){
				retryRect.y = STATE2;
				if( mEvent.button.button == SDL_BUTTON_LEFT && mEvent.button.state == SDL_PRESSED ){
					okRect.y = STATE3;
					printf("here we go!\n");
					renderAll();
					SDL_DestroyWindow( wgameOver );
					return 1;
					leave = 1;
				}
				renderAll();
			}else{
				retryRect.y = STATE1;
				renderAll();
			}	
		}
	}
}

void init_go(){
	SDL_Init( SDL_INIT_EVERYTHING );
	IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
	Mix_Init( MIX_INIT_MP3 );

	wgameOver = SDL_CreateWindow( "Game over", 
	SDL_WINDOWPOS_UNDEFINED, 
	SDL_WINDOWPOS_UNDEFINED, 
	500,
	300,
	SDL_WINDOW_BORDERLESS
	);

	go_render = SDL_CreateRenderer( wgameOver, -1, SDL_RENDERER_ACCELERATED );
	gameoverBg = IMG_LoadTexture( go_render, "./Img/GAME OVER.jpg" );
	okButton = IMG_LoadTexture( go_render, "./Img/QUIT.png" );
	retryButton = IMG_LoadTexture( go_render, "./Img/RETRY.png" );
	renderAll();
}

void renderAll(){
	SDL_RenderClear( go_render );
	SDL_RenderCopy( go_render, gameoverBg, NULL, NULL );
	SDL_RenderCopy( go_render, okButton, &okRect, &okPos );
	SDL_RenderCopy( go_render, retryButton, &retryRect, &retryPos );
	SDL_RenderPresent( go_render );
}
