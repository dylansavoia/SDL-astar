#include "a_star.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#include "heap.h"
#include "linked_list.h"


COLOR CLR_VISIT = {80, 80, 80};
COLOR CLR_EXP   = {50, 50, 50};

LinkedList *a_star(
    A_Vertex *graph[], unsigned n_verts, unsigned s, unsigned d,
    SDL_Renderer *render, GRID *grid
)
{
	// Init Globals
    Heap *open_list = heap_create();

    bool visited[n_verts] = {};

	//Init
	A_Vertex *source = graph[s];
    A_Vertex *curr_vert, *neigh_vert = NULL;

	curr_vert = source;
	float curr_vert_sum = 0, new_sum = 0;

	for (int i = 0; i < n_verts; ++i)
    {
        if (graph[i] == NULL) continue;

		graph[i] -> cost = 0;
		graph[i] -> sum = INT_MAX;
		graph[i] -> fpath = NULL;
	}

	source -> sum = 0;
    heap_push(open_list, 0, (void*) source);

	int x, y;
    int CELLX = grid -> CELLX;

    // While open_list isn't empty or we haven't yet found Destination...
	while (heap_size(open_list) > 0 && curr_vert -> id != d )
    { 
		curr_vert = (A_Vertex*) heap_pop(open_list);
        if (visited[curr_vert -> id]) continue;
        visited[curr_vert -> id] = true;

		x = curr_vert -> id % CELLX;
		y = curr_vert -> id / CELLX;


        *(COLOR*)(curr_vert -> data) = CLR_VISIT;

        // For each neighbor
		for (int j = 0; j < curr_vert -> n_edges; j++) {
            if (visited[curr_vert -> edges[j].id]) continue;

			neigh_vert = graph[curr_vert -> edges[j].id];
			new_sum = curr_vert -> cost + curr_vert -> edges[j].weight + neigh_vert -> heuristic;

			if ( new_sum < neigh_vert -> sum ){
				neigh_vert -> sum = new_sum;
				neigh_vert -> cost = curr_vert -> cost + curr_vert -> edges[j].weight;
				neigh_vert -> fpath = curr_vert;
			}

            heap_push(open_list, neigh_vert -> sum, neigh_vert);
            *(COLOR*)(neigh_vert -> data) = CLR_EXP;
		}   

        draw_grid(render, grid);
		SDL_Delay(5);
	}

    heap_destroy(open_list);

	// Build and return Path, starting from DEST and going backwards up to START
	A_Vertex *dest = graph[d];
    // If no path, return NULL
	if (dest -> fpath == NULL) return NULL;

    LinkedList *path = LL_create();
	while (dest != NULL && dest != source)
    {
		LL_push_front(path, (void*)(dest -> fpath));
        dest = dest -> fpath;
    }

	return path;
}
