#include "a_star.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#define INF 999999

extern int WIDTH, CELL_DIM;
extern SDL_Renderer *main_render;
extern GRID *grid;

A_LL_NODE *open_list, *closed_list;

A_Vertex **glb_graph;
unsigned glb_n_verts = 0;

//FUNCTIONS
A_Vertex *find_vertex( unsigned id ) {
	for (int i = 0; i < glb_n_verts; ++i)
		if ( glb_graph[i] -> id == id ) return glb_graph[i];
	return NULL;
}

void add_to_start ( A_LL_NODE *list, A_Vertex *vert_p ) {	
	A_LL_NODE *new_head = malloc(sizeof(A_LL_NODE));
	new_head -> vert_p = vert_p;
	new_head -> next = list -> next;
	list -> next = new_head;
}

void remove_elem ( A_LL_NODE *list, A_Vertex *vert_p ) {
	A_LL_NODE *prev = list;
	while ( (list = list -> next) != NULL ) {
		if ( list -> vert_p == vert_p ) {
			prev -> next = list -> next;
			free( list );
			return;
		}
		prev = list;
	};
}

A_Vertex *find_min ( A_LL_NODE *list ) {
	float curr_sum, min = INF;
	A_Vertex *min_p = NULL;

	while ( (list = list -> next) != NULL ) {
		curr_sum = list -> vert_p -> sum;
		if ( curr_sum <= min ) {
			min = curr_sum;
			min_p = list -> vert_p;
		}
	};
	return min_p;
}

void print_list ( A_LL_NODE *list ) {
	while ( (list = list -> next) != NULL ) {
		printf("%u\n", list -> vert_p -> id);
	};
}

int is_in_list ( A_LL_NODE *list, unsigned id ) {
	while ( (list = list -> next) != NULL ) {
		if ( list -> vert_p -> id == id ) return 1;
	};
	return 0;
}

void destroy_list ( A_LL_NODE *list ) {
	A_LL_NODE *prev = list;
	while ( list != NULL ) {
		list = list -> next;
		free(prev);
		prev = list;
	};
}

A_LL_NODE *a_star( A_Vertex *graph[], unsigned n_verts, unsigned s, unsigned d ) {
	//Init Global = 
	glb_graph = graph;
	glb_n_verts = n_verts;
	open_list = malloc(sizeof(A_LL_NODE));
	closed_list = malloc(sizeof(A_LL_NODE));
	open_list -> vert_p = closed_list -> vert_p = NULL;
	open_list -> next = closed_list -> next = NULL;

	//Init
	A_Vertex *source = find_vertex(s), *curr_vert, *cmp_vert = NULL;
	curr_vert = source;
	float curr_vert_sum = 0, new_sum = 0;

	for (int i = 0; i < n_verts; ++i) {
		graph[i] -> c = 0;
		graph[i] -> sum = INF;
		graph[i] -> fpath = NULL;
	}
	source -> sum = 0;
	add_to_start( open_list, source );

	int x, y, CELLX = (WIDTH / CELL_DIM);
	while ( open_list -> next != NULL && curr_vert -> id != d ){ //While open_list isn't empty...
		curr_vert = find_min(open_list);

		//printf("> %u\n", curr_vert -> id);

		add_to_start ( closed_list, curr_vert );
		remove_elem ( open_list, curr_vert );
		/*
		printf("\n\nopen_list:\n");
		print_list(open_list);
		printf("\nclosed_list:\n");
		print_list(closed_list);
		*/
		x = curr_vert -> id % CELLX;
		y = curr_vert -> id / CELLX;
        

        COLOR tmp1 = {50, 50, 50};
        COLOR tmp2 = {90, 150, 255};

        *(COLOR*)(curr_vert -> data) = tmp1;

		for (int j = 0; j < curr_vert -> n_edges; j++) {
			if ( is_in_list(closed_list, curr_vert -> edges[j].id) ) continue;

			cmp_vert = find_vertex(curr_vert -> edges[j].id);
			new_sum = curr_vert -> c + curr_vert -> edges[j].weight + cmp_vert -> h;

			if ( new_sum < cmp_vert -> sum ){
				cmp_vert -> sum = new_sum;
				cmp_vert -> c = curr_vert -> c + curr_vert -> edges[j].weight;
				cmp_vert -> fpath = curr_vert;
			}
			if ( !is_in_list(open_list, cmp_vert -> id) )
				add_to_start ( open_list, cmp_vert );
            *(COLOR*)(cmp_vert -> data) = tmp2;
		}   
        draw_grid(main_render, grid);
		SDL_Delay(5);
	}

	destroy_list(open_list);
	destroy_list(closed_list);

	A_Vertex *dest = find_vertex(d);
	if ( dest -> fpath == NULL )
		return NULL;

	//BUILD AND RETURN PATH
	A_LL_NODE *build_path = malloc(sizeof(A_LL_NODE));
	A_LL_NODE *path = NULL;
	build_path -> next = NULL;
	build_path -> vert_p = NULL;

	add_to_start(build_path, dest);
	while ( dest != NULL && dest != source )
		add_to_start(build_path, dest = dest -> fpath);

	path = build_path -> next;
	free(build_path);

	return path;
}
