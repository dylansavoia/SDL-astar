#ifndef ASTAR
#define ASTAR

typedef struct A_Edge {
	unsigned id;
	float weight;
} A_Edge;

typedef struct A_Vertex {
	unsigned id, n_edges;
	float c, //COST SO FAR FROM START NODE
	h, sum; //HEURISTIC, sum of c and h
	A_Edge *edges; //ARRAY OF EDGES
	struct A_Vertex *fpath;
	void *data;
} A_Vertex;

typedef struct A_LL_NODE {
	A_Vertex *vert_p;
	struct A_LL_NODE *next;
} A_LL_NODE;

A_LL_NODE *a_star ( A_Vertex *graph[], unsigned n_verts, unsigned source, unsigned destination );
#endif