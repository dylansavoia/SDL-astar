#ifndef ASTAR_H
#define ASTAR_H

#include <SDL2/SDL.h>
#include "grid.h"
#include "linked_list.h"

typedef struct A_Edge {
    unsigned id;
    float weight;
} A_Edge;

typedef struct A_Vertex {
    unsigned id, n_edges;
    float cost;             // COST SO FAR FROM START NODE
    float heuristic, sum;   // HEURISTIC, sum of c and heuristic
    A_Edge *edges;          // ARRAY OF EDGES
    struct A_Vertex *fpath;
    void *data;
} A_Vertex;

LinkedList *a_star (
    A_Vertex *graph[], unsigned n_verts, unsigned source, unsigned destination,
    SDL_Renderer *render, GRID *grid
);


#endif
