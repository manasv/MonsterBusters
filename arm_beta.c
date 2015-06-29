#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_video.h"

#define WINDOW_SIZE 600
#define WINDOW_CENTER_W (WINDOW_SIZE/2) //tomar en cuenta las dimensiones de la imagen!
#define WINDOW_CENTER_H (WINDOW_SIZE/2) //por lo tanto separar altura de ancho

#define CART_X(x) x + WINDOW_CENTER_W
#define CART_Y(y) WINDOW_SIZE - (WINDOW_CENTER_H + y)

#define PI 3.141592
#define RAD_TO_DEG (180.0/PI)

#define THETA 0.174532925

float rot_pos[2][2] = {
	{cos(THETA), -sin(THETA)},
	{sin(THETA),  cos(THETA)}
};

float rot_neg[2][2] = {
	{cos(-THETA), -sin(-THETA)},
	{sin(-THETA),  cos(-THETA)}
};

//no entiendo por que -65 y -20 pero eso lo pone en el centro aparente
//quizas por que el eje esta en el centro de la imagen y no en el circulo
//de la punta! puede ser!

typedef struct {
	double x;
	double y;
} vector;

typedef struct {
	SDL_Texture *texture;
	int h;
	int w;
} GAME_Texture;

typedef struct {
	GAME_Texture t;
	vector pos;
} Arm_Segment;

typedef struct {
	Arm_Segment segments[3];
} GAME_Arm;

void v_add(vector *a, vector *b, vector *c) {
	c->x = a->x + b->x;
	c->y = a->y + b->y;
}

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

GAME_Texture bg_t;
GAME_Texture axis_t;

GAME_Arm arm;

void load_texture_from_image(char *filename, GAME_Texture *tex) {
	SDL_Surface *s;
	s = IMG_Load(filename);
	tex->texture = SDL_CreateTextureFromSurface(renderer, s);
	tex->w = s->w;
	tex->h = s->h;
	printf("Loaded '%s', (%d, %d)\n", filename, s->w, s->h);
	SDL_FreeSurface(s);
}

void load_images() {
	load_texture_from_image("Img/background.jpg", &bg_t);
	
	// Load arm
	int i;
	char filename[255];
	for(i = 0; i < 3; i++) {
		sprintf(filename, "Img/arm-%d.png", i + 1);
		load_texture_from_image(filename, &(arm.segments[i].t));
		arm.segments[i].pos.x = 1000.0;
		arm.segments[i].pos.y = 0.0;
	}

	load_texture_from_image("./Img/axis.png", &axis_t);
}

void draw_game() {
	// SDL_Rect arm_rect  = {CART_X(axis->w/2), CART_Y(arm_sur->h/2), arm_sur->w, arm_sur->h};
	// SDL_Point center   = {-axis->w/2, arm_sur->h/2}; //Cambio a respectiva definicion
	
	SDL_Rect axis_rect = {CART_X(-(axis_t.w/2)), CART_Y(axis_t.h/2), axis_t.w, axis_t.h};
	//Cambio a respectiva definicion
 	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg_t.texture, NULL, NULL);

    SDL_Rect arm_rect;
    int rect_offset_x = axis_t.w/2, rect_offset_y = 0;
	SDL_Point arm_center = {-axis_t.w/2, 0.0};
	float angle_rad, angle_deg;
	double h;
	int i;
	for(i = 0; i < 3; i++) {
    	Arm_Segment *seg = &(arm.segments[i]);
    	arm_rect.x = CART_X(rect_offset_x);
    	arm_rect.y = CART_Y(rect_offset_y + seg->t.h/2);
    	arm_rect.w = seg->t.w;
    	arm_rect.h = seg->t.h;
    	
    	arm_center.y = seg->t.h/2;
    	angle_rad = atan(seg->pos.y/seg->pos.x);
		angle_deg = angle_rad * RAD_TO_DEG;
		
    	if( seg->pos.x < 0 ) {
			angle_deg = angle_deg + 180;
			angle_rad = - angle_rad;
    	}
    	SDL_RenderCopyEx(renderer, seg->t.texture, NULL, &arm_rect, -angle_deg, &arm_center, SDL_FLIP_NONE);
    	arm_center.x = - seg->t.h/2;
    	
    	rect_offset_x += seg->t.w * cos(angle_rad);
    	rect_offset_y += seg->t.w * sin(angle_rad);
    }
    SDL_RenderCopy(renderer, axis_t.texture, NULL, &axis_rect);
	SDL_RenderPresent(renderer);
}

/*
x' = x \cos \theta - y \sin \theta\,,
y' = x \sin \theta + y \cos \theta\,.
*/
void rotate_arm(int start_seg, float theta) {
	int i;
	for(i = start_seg; i < 3; i++) {
		Arm_Segment *seg = &(arm.segments[i]);
		int x = seg->pos.x, y = seg->pos.y;
		seg->pos.x = x * cos(theta) - y * sin(theta);
		seg->pos.y = x * sin(theta) + y * cos(theta);
		// printf("Seg[%d] = (%.2f, %.2f)\n", i, seg->pos.x, seg->pos.y);
	}
}

int main( int argc, char** argv ) { //cambie esto de main(void) a lo que esta ahorita
	int finish = 0;
	double angle = 0.0;
	SDL_Event evt;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, 0, &window, &renderer);
	load_images();
	draw_game();

	while(!finish) {
		if(SDL_WaitEvent(&evt)) {
			switch(evt.type) {
				case SDL_QUIT:
					finish = 1;
					break;
				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_RIGHT) {
						rotate_arm(0, -THETA);
					} else if(evt.key.keysym.sym == SDLK_LEFT) {
						rotate_arm(0, THETA);
					} else if(evt.key.keysym.sym == SDLK_UP) {
						rotate_arm(1, THETA);
					} else if(evt.key.keysym.sym == SDLK_DOWN) {
						rotate_arm(1, -THETA);
					}  else if(evt.key.keysym.sym == SDLK_w) {
						rotate_arm(2, THETA);
					} else if(evt.key.keysym.sym == SDLK_s) {
						rotate_arm(2, -THETA);
					}
					draw_game();
					break;
			}
		}
	}
	SDL_Quit();
	return 0;
}
