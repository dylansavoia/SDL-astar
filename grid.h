#include <SDL2/SDL.h>
#ifndef GRIDLIB
#define GRIDLIB

typedef struct COLOR {
	unsigned char r, g, b;
} COLOR;

typedef struct CELL {
	SDL_Rect *rect;
	COLOR clr;
} CELL;

typedef struct GRID {
	unsigned x, y, w, h;
	unsigned CELLX, CELLY;
	CELL ***cells;
} GRID;

GRID *init_grid( unsigned x, unsigned y, unsigned w, unsigned h, unsigned cell_dim );
void delete_grid( GRID *grid, unsigned CELLX, unsigned CELLY );
void draw_grid( SDL_Renderer *render, GRID *grid );
#endif

