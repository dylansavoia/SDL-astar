#ifndef GRID_H
#define GRID_H

#include <SDL2/SDL.h>

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

GRID *grid_init(unsigned x, unsigned y, unsigned w, unsigned h, unsigned cell_dim, COLOR clr);
void grid_destroy(GRID *grid, unsigned CELLX, unsigned CELLY);
void grid_draw(SDL_Renderer *render, GRID *grid);

void grid_save (GRID *grid, char *pathname, COLOR *wall_clr);
void grid_load (char *pathname, GRID *grid, COLOR *wall_clr);

#endif
