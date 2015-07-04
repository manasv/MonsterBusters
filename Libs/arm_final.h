//#include <stdio.h>  only used when stand alone!
//#include <unistd.h>  only used when stand alone!
//#include <math.h>  only used when stand alone!
//#include <SDL2/SDL.h>  only used when stand alone!
//#include <SDL2/SDL_image.h>  only used when stand alone!
//#include <SDL2/SDL_video.h>  only used when stand alone!

#define WINDOW_HEIGHT 640
#define WINDOW_CENTER (WINDOW_HEIGHT/2)
#define SEGMENT_COUNT 3

#define PRINT_VEC(name, v) printf("%s Vector(%.2f, %.2f)\n", name, (v)->x, (v)->y)

#define PI 3.141592654

#define ANGLE_STEP 0.174532925 // 10 grados

typedef struct {
	double x;
	double y;
	double z;
} vector;

typedef struct {
	SDL_Texture *texture;
	int h;
	int w;
} GAME_Texture;

typedef struct {
	GAME_Texture t;
	vector position;
	vector orientation;
} GAME_Object;

typedef struct {
	GAME_Object segments[SEGMENT_COUNT];
	double angles[SEGMENT_COUNT];
} GAME_Arm;

GAME_Texture bg_t;
GAME_Object axis_obj;

GAME_Arm arm;

double **rotat_mat = NULL;
double **trans_mat = NULL;

vector translated_pos;
vector rotated_pos;
vector relative_pos = {0.0, 0.0};
SDL_Rect obj_rect;
double limits[] = {0.0, 90.0, 90.0};
double angle, angle_rad;
double valuex, valuey;

double calculate_vector_angle(vector *v) {
	double radians = acos(v->x/sqrt(pow(v->x, 2.0) + pow(v->y, 2.0)));
	float degrees = (radians * 180.0)/PI;
	// printf("%.2f, y = %.2f\n", degrees, v->y);
	return ( v->y > 0.0 )? degrees : 360.0 - degrees; 
}

void matrix_mult(int size, double *in, double **mat, double *out) {
	int i, j;
	double acc;
	for(i = 0; i < size; i++) {
		acc = 0.0;
		for(j = 0; j < size; j++) {
			acc += in[j] * mat[j][i];
		}
		out[i] = acc;
	}
}

void populate_trans_mat(double **m, int x, int y, int z) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(i == j) {
				m[j][i] = 1.0;
			} else {
				m[j][i] = 0.0;
			}
		}
	}
	m[3][0] = (float)x;
	m[3][1] = (float)y;
	m[3][2] = (float)z;
}

void populate_rotat_mat(double **m, double rad_angle) {
	int i, j;
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			m[j][i] = 0.0;
		}
	}
	m[0][0] =  cos(rad_angle);
	m[1][0] =  sin(rad_angle);
	m[0][1] = -sin(rad_angle);
	m[1][1] =  cos(rad_angle);
	m[2][2] = 1.0;
}

void load_texture_from_image(char *filename, GAME_Texture *tex, SDL_Renderer* renderer) {
	SDL_Surface *s;
	s = IMG_Load(filename);
	tex->texture = SDL_CreateTextureFromSurface(renderer, s);
	tex->w = s->w;
	tex->h = s->h;
	SDL_FreeSurface(s);
	printf("%s -> (%d, %d)\n", filename, tex->w, tex->h);
}

void load_images( SDL_Renderer* renderer ) {
	int i;
	char filename[255];

	load_texture_from_image("./Img/axis.png", &(axis_obj.t), renderer);
	axis_obj.position.x = -(axis_obj.t.w/2);
	axis_obj.position.y = -(axis_obj.t.h/2);

	for(i = 0; i < SEGMENT_COUNT; i++) {
		sprintf(filename, "Img/arm-%d.png", i + 1);
		load_texture_from_image(filename, &(arm.segments[i].t), renderer);
		arm.segments[i].position.x = 0;
		arm.segments[i].position.y = 0; //-arm.segments[i].t.h/2;
		arm.segments[i].orientation.x = 1.0;
		arm.segments[i].orientation.y = 0.0;		
		arm.angles[i] = 0.0;
	}

}

void rotate_vector(vector *pos, double angle, vector *result) {
	populate_rotat_mat(rotat_mat, angle);
	double in[] = {pos->x, pos->y, pos->z};
	double out[3];
	matrix_mult(3, in, rotat_mat, out);
	result->x = out[0];
	result->y = out[1];
	result->z = out[2];
}

void translate_vector(vector *pos, int x, int y, vector *result) {
	populate_trans_mat(trans_mat, x, y, 0.0);
	double in[] = {pos->x, pos->y, pos->z, 1.0};
	double out[4];
	matrix_mult(4, in, trans_mat, out);
	result->x = out[0];
	result->y = out[1];
	result->z = out[2];
}

void draw_arm( SDL_Renderer* renderer ) {
	int i;
	translated_pos;
	rotated_pos;
	relative_pos.x = 0.0;
	relative_pos.y = 0.0;
	SDL_Point center = {-13, 0.0};

	translate_vector(&(axis_obj.position), WINDOW_CENTER, WINDOW_CENTER, &translated_pos);
	obj_rect.w = axis_obj.t.w;
	obj_rect.h = axis_obj.t.h;
	obj_rect.x = translated_pos.x;
	obj_rect.y = translated_pos.y;
	SDL_RenderCopy(renderer, axis_obj.t.texture, NULL, &obj_rect);

	for(i = 0; i < SEGMENT_COUNT; i++) {
		GAME_Object *seg = &(arm.segments[i]);
		translate_vector(&(seg->position), WINDOW_CENTER + relative_pos.x + 13, WINDOW_CENTER + relative_pos.y, &translated_pos);
		//printf("Translated (%.2f, %.2f)\n", rotated_pos.x,rotated_pos.y);
		rotate_vector(&(seg->orientation), arm.angles[i], &rotated_pos);
		angle = calculate_vector_angle(&rotated_pos);
		
		angle_rad = (angle/180.0) * PI;
		obj_rect.w = seg->t.w;
		obj_rect.h = seg->t.h;
		obj_rect.x = translated_pos.x;
		obj_rect.y = translated_pos.y - seg->t.h/2;
		center.y = seg->t.h/2;

		
		SDL_RenderCopyEx(renderer, seg->t.texture, NULL, &obj_rect, angle, &center, SDL_FLIP_NONE);
		center.x = - seg->t.h/2;

		relative_pos.x += seg->t.w * cos(angle_rad);
		relative_pos.y += seg->t.w * sin(angle_rad);
	}
}

void draw_game( SDL_Renderer* renderer ) {
	SDL_RenderClear( renderer );
    draw_arm( renderer );
    SDL_RenderPresent(renderer);
}

void init_matrix() {
	int i;
	rotat_mat = (double **)malloc(sizeof(double*) * 3);
	for(i = 0; i < 3; i++) {
		rotat_mat[i] = (double*)malloc(sizeof(double) * 3);
	}
	trans_mat = (double **)malloc(sizeof(double*) * 4);
	for(i = 0; i < 4; i++) {
		trans_mat[i] = (double*)malloc(sizeof(double) * 4);
	}
}

void update_angle(int start, double theta) {
	int i;
	double angle;
	for(i = start; i < SEGMENT_COUNT; i++) {
		angle = arm.angles[i] + theta;
		if(angle > 2 * PI) {
			arm.angles[i] = angle - 2 * PI;
		} else if (angle < 0) {
			arm.angles[i] = 2 * PI + angle;
		} else {
			arm.angles[i] = angle;
		}
	}
}

int arm_general( SDL_Renderer* renderer ) { //cambie esto de main(void) a lo que esta ahorita
	int finish = 0;
	SDL_Event evt;
	init_matrix();
	load_images( renderer );
	draw_game( renderer );

	if(!finish) {
		if(SDL_WaitEvent(&evt)) {
			switch(evt.type) {
				case SDL_QUIT:
					finish = 1;
					break;
				case SDL_KEYDOWN:
					if(evt.key.keysym.sym == SDLK_RIGHT) {
						update_angle(0, -ANGLE_STEP);
					} else if(evt.key.keysym.sym == SDLK_LEFT) {
						update_angle(0, ANGLE_STEP);
					} else if(evt.key.keysym.sym == SDLK_UP) {
						update_angle(1, ANGLE_STEP);
					} else if(evt.key.keysym.sym == SDLK_DOWN) {
						update_angle(1, -ANGLE_STEP);
					}  else if(evt.key.keysym.sym == SDLK_w) {
						update_angle(2, ANGLE_STEP);
					} else if(evt.key.keysym.sym == SDLK_s) {
						update_angle(2, -ANGLE_STEP);
					}
					draw_game( renderer );
					break;
			}
		}
	}
	return 0;
}
