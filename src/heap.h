#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

struct _heap_node {
    int value;
    void *payload;
};

typedef struct Heap {
    struct _heap_node *arr;
    size_t capacity;
} Heap;

Heap *heap_create ();
void heap_destroy (Heap *p);

void heap_push(Heap *heap, int value, void *payload);
void *heap_pop(Heap *heap);

void heapify_up   (Heap *heap, int idx);
void heapify_down (Heap *heap, int idx);

// Utilities
void heap_swap (Heap *heap, int a, int b);
void heap_print (Heap *heap);
size_t heap_size (Heap *heap);


#endif
