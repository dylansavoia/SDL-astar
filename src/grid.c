#include <SDL2/SDL.h>
#include <stdlib.h>
#include "grid.h"
#include <stdbool.h>

GRID *grid_init( unsigned x, unsigned y, unsigned w, unsigned h, unsigned cell_dim, COLOR clr) {
	unsigned CELLX = w / cell_dim, CELLY = h / cell_dim;

	GRID *new_grid = malloc(sizeof(GRID));
	new_grid->x = x; new_grid->y = y;
	new_grid->w = w; new_grid->h = h;

	new_grid->CELLX = CELLX;
    new_grid->CELLY = CELLY;

	CELL ***matrix = malloc(CELLY*sizeof(void*));
	CELL *new_cell = NULL;

	for( int i = 0; i < CELLY; i++ ) {
		matrix[i] = malloc(CELLX*sizeof(void*));
		for ( int j = 0; j < CELLX; j++ ) {
			new_cell = malloc(sizeof(CELL));
			new_cell -> rect = malloc(sizeof(SDL_Rect));
			new_cell -> rect -> x = j * cell_dim + x;
			new_cell -> rect -> y = i * cell_dim + y;
			new_cell -> rect -> w = new_cell -> rect -> h = cell_dim;
			new_cell -> clr = clr;
			matrix[i][j] = new_cell;
		}
	}
	new_grid -> cells = matrix;
	return new_grid;
}

void grid_destroy( GRID *grid, unsigned CELLX, unsigned CELLY ) {
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

void grid_draw( SDL_Renderer *rend, GRID *grid ) {
	CELL *curr_cell = NULL;
	for (int i = 0; i < grid -> CELLY; i++)
	for (int j = 0; j < grid -> CELLX; j++)
    {
        curr_cell = grid->cells[i][j];
        SDL_SetRenderDrawColor( rend, curr_cell->clr.r, curr_cell->clr.g, curr_cell->clr.b, 255 );
        SDL_RenderFillRect( rend, curr_cell->rect);
	}

    SDL_RenderPresent(rend);
}

void grid_save (GRID *grid, char *pathname, COLOR *wall_clr)
{
    FILE *file = fopen(pathname, "w");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
	CELL *curr_cell = NULL;
	for (int i = 0; i < grid -> CELLY; i++)
	for (int j = 0; j < grid -> CELLX; j++)
    {
        curr_cell = grid -> cells[i][j];
        bool is_wall = curr_cell -> clr.r == wall_clr -> r &&
                       curr_cell -> clr.g == wall_clr -> g &&
                       curr_cell -> clr.b == wall_clr -> b;

        fputc('0' + is_wall, file);
	}

    fclose(file);
    printf("Grid Saved: %s\n", pathname);
}

void grid_load (char *pathname, GRID *grid, COLOR *wall_clr)
{
    FILE *file = fopen(pathname, "r");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    unsigned count = 0;
    int CELLX = grid -> CELLX;
    int CELLY = grid -> CELLY;

	CELL *curr_cell = NULL;

    int is_wall = 0;
    while ((is_wall = getc(file)) != EOF )
    {
        int i = count / CELLX;
        int j = count % CELLX;

        if (count >= CELLX * CELLY) break;
        count++;

        curr_cell = grid -> cells[i][j];
        curr_cell -> clr.r = (is_wall == '0') ? 0 : wall_clr -> r;
        curr_cell -> clr.g = (is_wall == '0') ? 0 : wall_clr -> g;
        curr_cell -> clr.b = (is_wall == '0') ? 0 : wall_clr -> b;
    }

    fclose(file);
    printf("Grid Loaded: %s\n", pathname);
}


