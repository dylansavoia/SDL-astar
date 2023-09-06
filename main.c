#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "grid.h"
#include "a_star.h"
#include <time.h>
#include <sys/wait.h>

int WIDTH = 1200, HEIGHT = 880, CELL_DIM = 20;

unsigned char main_colors[4][3] = {
	{200, 200, 200},
	{0, 128, 0},
	{128, 0, 0},
};
COLOR white = {200, 200, 200};
COLOR black = {0, 0, 0};
COLOR green = {0, 128, 0};
COLOR red = {128, 0, 0};
COLOR path_clr = {0, 50, 200};
COLOR path_clr2 = {200, 60, 0};
A_LL_NODE *path = NULL;

SDL_Window *main_win = NULL;
SDL_Renderer *main_render = NULL;
SDL_Event evt;
GRID *grid;
int s_pos[] = {-1, -1}, d_pos[] = {-1, -1};

//Matrix to Linear index
unsigned mtol( unsigned i, unsigned j, unsigned width );
void find_path (GRID *grid);
void restore_grid (GRID *grid);
int is_wall(GRID *grid, int i, int j);

int main() {
	/*
		0: draw walls
		1: set source
		2: set destination
	*/
	int mode = 0;

	if ( SDL_Init(SDL_INIT_EVERYTHING) ) return -1;
	main_win = SDL_CreateWindow( "Main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0 );
	main_render = SDL_CreateRenderer( main_win, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(main_render, WIDTH, HEIGHT);

	grid = init_grid(0, 0, WIDTH, HEIGHT, CELL_DIM);

	char main_bool = 1;
	COLOR curr;
	int x, y, btn;
	//Main_Loop
	while ( main_bool ) {
		while ( SDL_PollEvent(&evt) ) {
			if ( evt.type == SDL_QUIT ) main_bool = 0;
			else if ( evt.type == SDL_KEYUP ) {
				switch (evt.key.keysym.sym) {
					case SDLK_s: 
						mode = 1; break;
					case SDLK_d: 
						mode = 2; break;
					case SDLK_w: 
						mode = 0; break;
				}
            	if ( evt.key.keysym.sym == SDLK_RETURN || evt.key.keysym.sym == SDLK_KP_ENTER ) {
            		curr = grid -> cells[s_pos[1]][s_pos[0]] -> clr;
            		if ( curr.r == path_clr.r && curr.g == path_clr.g && curr.b == path_clr.b)
            			restore_grid(grid);
            		else find_path (grid);
            	};
			} else if ( evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEMOTION ) {
                x = evt.button.x / CELL_DIM;
                y = evt.button.y / CELL_DIM;
                btn = evt.button.button;

                if ( btn == 1 ) {
                	grid -> cells[y][x] -> clr.r = main_colors[mode][0];
                	grid -> cells[y][x] -> clr.g = main_colors[mode][1];
                	grid -> cells[y][x] -> clr.b = main_colors[mode][2];

                	if ( mode == 1 ) {s_pos[0] = x; s_pos[1] = y;}
                	else if ( mode == 2 ) {d_pos[0] = x; d_pos[1] = y;}
                } else if ( btn == 4 ) {
                	grid -> cells[y][x] -> clr.r = main_colors[3][0];
                	grid -> cells[y][x] -> clr.g = main_colors[3][1];
                	grid -> cells[y][x] -> clr.b = main_colors[3][2];
                }
            }
		}
		SDL_SetRenderDrawColor(main_render, 128, 128, 128, 255);
		SDL_RenderClear(main_render);
		draw_grid(main_render, grid);
		SDL_Delay(100);
	}
	SDL_Quit();
	delete_grid( grid, WIDTH / CELL_DIM, HEIGHT / CELL_DIM );
	return 0;
} 

void find_path ( GRID *grid ) {
	unsigned CELLX = grid -> CELLX,
			CELLY = grid -> CELLY,
			n_verts = CELLX * CELLY,
			n_edges = 0, curr_vert = 0;

	A_Vertex *verts[n_verts], *new_vert = NULL;
	A_Edge *new_edges = NULL;
	for (int i = 0; i < grid -> CELLY; i++) {
		for (int j = 0; j < grid -> CELLX; j++) {
			if ( is_wall(grid, i, j) ) continue;

			new_edges = malloc( 8 * sizeof(A_Edge) );
			n_edges = 0;
			for (int sub_i = i-1; sub_i <= i+1; ++sub_i) {
				for (int sub_j = j-1; sub_j <= j+1; ++sub_j) {
					if ( sub_i < 0 || sub_i > CELLY -1) continue;
					if ( sub_j < 0 || sub_j > CELLX -1 ) continue;
					if ( sub_i == i && sub_j == j ) continue;
					if ( (sub_j == j || sub_i == i) && !is_wall( grid, sub_i, sub_j ) ) {
					//if ( !is_wall( grid, sub_i, sub_j ) ) {
						new_edges[n_edges].id = mtol(sub_i, sub_j, CELLX);
						//if (sub_j == j || sub_i == i) {
							new_edges[n_edges++].weight = 1;
						//} else new_edges[n_edges++].weight = 1.4;
					}
				}
			}
			new_edges = realloc( new_edges, n_edges * sizeof(A_Edge) );

			new_vert = malloc(sizeof(A_Vertex));
			new_vert -> id = mtol(i, j, CELLX );
			new_vert -> data = &(grid -> cells[i][j] -> clr);
			//sqrt( pow(d_pos[1] - i,2) + pow(d_pos[0]-j,2) );
			//abs( d_pos[1] - i) + abs( d_pos[0]-j );
			new_vert -> h = 1.5*(abs( d_pos[1] - i) + abs( d_pos[0]-j ));
			new_vert -> edges = new_edges;
			new_vert -> n_edges = n_edges;
			verts[curr_vert++] = new_vert;
		}

	}

	
	while ( path != NULL ) {
		free(path -> vert_p);
		path = path -> next;
	}
	

	clock_t start, end;
	double cpu_time_used;

	unsigned start_id = mtol(s_pos[1], s_pos[0], CELLX),
		end_id = mtol(d_pos[1], d_pos[0], CELLX);

	start = clock();
	path = a_star( verts, curr_vert, start_id, end_id);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("\nTIME: %lf\n", cpu_time_used);
	
	if ( path == NULL ) {
        printf("No Path Found\n");
		grid -> cells[s_pos[1]][s_pos[0]] -> clr = green;
        restore_grid(grid);
    } else {
		int x, y;
		int prev_x = s_pos[0], prev_y = s_pos[1];
		A_LL_NODE *curr_node = path;

		while ( curr_node != NULL ) {
			x = curr_node -> vert_p -> id % grid -> CELLX;
			y = curr_node -> vert_p -> id / grid -> CELLX;
			
			grid -> cells[prev_y][prev_x] -> clr = path_clr;
			grid -> cells[y][x] -> clr = path_clr2;
            draw_grid(main_render, grid);

            SDL_Delay(25);


			prev_x = x;
			prev_y = y;
			curr_node = curr_node -> next;
		}

		//Free Memory
		/*int is_path = 0;
		for (int i = 0; i < curr_vert; i++) {
			free( verts[i] -> edges );
			is_path = 0;
			for (int j = 0; path[j] != NULL; j++)
				if ( verts[i] == path[j] )
					is_path = 1; break;
			if ( !is_path ) free( verts[i] );
		}*/
	}
}

int is_wall (GRID *grid, int i, int j) {
	COLOR curr = grid -> cells[i][j] -> clr;
	if ( curr.r == white.r && curr.g == white.g && curr.b == white.b) return 1;
	return 0;
}

void restore_grid ( GRID *grid ) {
	int x, y;

	A_LL_NODE *curr_node = path;
	while ( curr_node != NULL ) {
		x = curr_node -> vert_p ->id % grid -> CELLX;
		y = curr_node -> vert_p ->id / grid -> CELLX;
		if ( x == s_pos[0] && y == s_pos[1] )
			grid -> cells[y][x] -> clr = green;
		else if ( x == d_pos[0] && y == d_pos[1] )
			grid -> cells[y][x] -> clr = red;
		else
			grid -> cells[y][x] -> clr = black;
		curr_node = curr_node -> next;
	}

	for( int i = 0; i < grid -> CELLY; i++ ) {
		for ( int j = 0; j < grid -> CELLX; j++ ) {
            COLOR curr = grid -> cells[i][j] -> clr;
            if ( (curr.r == 50 && curr.g == 50 && curr.b == 50) ||
                curr.r == 90 && curr.g == 150 && curr.b == 255 ) {
                grid -> cells[i][j] -> clr = black; 
            }
		}
	}
}

unsigned mtol( unsigned i, unsigned j, unsigned width ) {
	return i * width + j; 
}

