#include <SDL2/SDL.h>
#include <unistd.h> //rand() esta aqui!
#include <time.h> //time_t

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500

int main( int argc, char** argv ){
	time_t t;
	srand( (unsigned) time(&t) ); //genera la semilla del random
	int i = 0, j = 255, k = rand()%255; //numero random entre 255 y 0
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL; //permitira dibujar en ventana
	if( SDL_Init(SDL_INIT_VIDEO) ){
		fprintf( stderr,"\nNo se pudo inciar SDL: %s\n",SDL_GetError() );
		return 1;
	}
	window = SDL_CreateWindow(
		"Nakireamendayo!", //nombre de la ventana
		SDL_WINDOWPOS_UNDEFINED, //Posicion incial en X
		SDL_WINDOWPOS_UNDEFINED, //Posicion inicial en Y
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL //banderas, utiliza opengl
	);
	//verificar si la ventana fue creada
	if( window == NULL ){
		fprintf( stderr,"no se pudo crear la ventana: %s", SDL_GetError() );
		return 1;
	}
	while(i < 255){
		surface = SDL_GetWindowSurface( window ); //aplico la superficie a la pantalla
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format,i, k, j));
		SDL_UpdateWindowSurface( window );
		SDL_Delay( 10 );
		i++; //i aumenta, es el rojo
		j--; //j disminuye, es el azul
	}
	//eso aplico el color, ahora a refrescar, moradito gay btw
	SDL_Delay( 2000 ); //se detiene el programa un segundo
	//Destruir ventana!
	SDL_FreeSurface( surface );
	SDL_DestroyWindow( window );
	surface = NULL;
	window = NULL;
	//se limpian los subsitemas de SDL
	SDL_Quit();
	return 0;
}
