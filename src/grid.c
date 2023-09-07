#include <SDL2/SDL.h>
#include <stdlib.h>
#include "grid.h"

GRID *init_grid( unsigned x, unsigned y, unsigned w, unsigned h, unsigned cell_dim ) {
	unsigned CELLX = w / cell_dim, CELLY = h / cell_dim;

	GRID *new_grid = malloc(sizeof(GRID));
	new_grid->x = x; new_grid->y = y;
	new_grid->w = w; new_grid->h = h;
	new_grid->CELLX = CELLX; new_grid->CELLY = CELLY;

	CELL ***matrix = malloc(CELLY*sizeof(void*));
	CELL *new_cell = NULL;
	COLOR black = {0 ,0 ,0};

	for( int i = 0; i < CELLY; i++ ) {
		matrix[i] = malloc(CELLX*sizeof(void*));
		for ( int j = 0; j < CELLX; j++ ) {
			new_cell = malloc(sizeof(CELL));
			new_cell -> rect = malloc(sizeof(SDL_Rect));
			new_cell -> rect -> x = j * cell_dim + x;
			new_cell -> rect -> y = i * cell_dim + y;
			new_cell -> rect -> w = new_cell -> rect -> h = cell_dim;
			new_cell -> clr = black;
			matrix[i][j] = new_cell;
		}
	}
	new_grid -> cells = matrix;
	return new_grid;
}

void delete_grid( GRID *grid, unsigned CELLX, unsigned CELLY ) {
	for( int i = 0; i < CELLY; i++ ) {
		for ( int j = 0; j < CELLX; j++ ) {
			free(grid -> cells[i][j] -> rect);
			free(grid -> cells[i][j]);
		}
		free(grid -> cells[i]);
	}
	free(grid ->  cells);
	free(grid);
}

void draw_grid( SDL_Renderer *rend, GRID *grid ) {
	CELL *curr_cell = NULL;
	for( int i = 0; i < grid -> CELLY; i++ ) {
		for ( int j = 0; j < grid -> CELLX; j++ ) {
			curr_cell = grid->cells[i][j];
			SDL_SetRenderDrawColor( rend, curr_cell->clr.r, curr_cell->clr.g, curr_cell->clr.b, 255 );
			SDL_RenderFillRect( rend, curr_cell->rect);
		}
	}
    SDL_RenderPresent(rend);
}
