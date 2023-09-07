#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

Heap *heap_create ()
{
    Heap *new_heap  = malloc(sizeof(Heap));
    new_heap -> capacity = 10;

    // 0-th element is not a real value, it's the size of the heap
    new_heap -> arr = malloc(11 * sizeof(struct _heap_node));
    new_heap -> arr[0].value = 0;

    return new_heap;
}

void heap_destroy (Heap *p)
{
    free(p -> arr);
    free(p);
}

void heap_push(Heap *heap, int value, void *payload)
{
    // Increase size of heap
    size_t heap_size = ++(heap -> arr[0].value);   

    // Double the size of the array when capacity is reached
    if (heap_size == heap -> capacity)
    {
        struct _heap_node *prev_arr = heap -> arr;
        heap -> capacity *= 2;

        // Always 1 plus the actual capacity (0th element is the size of the heap)
        heap -> arr    = malloc((heap -> capacity +1) * sizeof(struct _heap_node));
        for (int i = 0; i < heap_size; i++)
            heap -> arr[i] = prev_arr[i];

        free(prev_arr);
    }

    heap -> arr[heap_size].value   = value;
    heap -> arr[heap_size].payload = payload;

    heapify_up(heap, heap_size);
}

void *heap_pop(Heap *heap)
{
    if (heap -> arr[0].value == 0) return NULL;

    size_t heap_size = heap -> arr[0].value--;   // Decrease size of heap
    void *ret_data   = heap -> arr[1].payload;

    // Move last element to first
    heap -> arr[1].value   = heap -> arr[heap_size].value;
    heap -> arr[1].payload = heap -> arr[heap_size].payload;

    // Heapify from first element
    heapify_down(heap, 1);
    
    return ret_data;
}

void heapify_up (Heap *heap, int idx)
{
    while (idx > 1) 
    {
        int parent_idx = idx / 2;
        struct _heap_node *parent = heap -> arr + parent_idx;
        struct _heap_node *curr   = heap -> arr + idx;

        // Swap if child is lower then element
        if (curr -> value <= parent -> value)
            heap_swap(heap, idx, parent_idx);

        idx = parent_idx;
    }
}

void heapify_down (Heap *heap, int idx)
{
    size_t heap_size = heap -> arr[0].value;
    
    // Loop until we reach a leaf (node without neither left or right)
    while (idx * 2 <= heap_size)
    {
        // Because of the condition above, left is always defined
        struct _heap_node *left   = heap -> arr + (idx * 2);

        // Right may or may not be defined
        int right_idx = idx * 2 + 1;
        struct _heap_node *right  = NULL;

        if (right_idx <= heap_size)
            right = heap -> arr + right_idx;

        // Get smallest child index
        int min_idx;
        min_idx = (right == NULL || left -> value < right -> value) ? idx * 2 : idx * 2 +1;

        // If curr node is already the lowest, nothing to do
        if (heap -> arr[idx].value <
            heap -> arr[min_idx].value) return;
       
        // Otherwise, swap and propagate below
        heap_swap(heap, idx, min_idx);

        idx = min_idx;
    }

}

void heap_print (Heap *heap)
{
    size_t heap_size = heap -> arr[0].value;   

    for (int i = 1; i <= heap_size; i++)
        printf("%d ", heap -> arr[i].value);

    printf("\n");
}

void heap_swap (Heap *heap, int a, int b)
{
    struct _heap_node tmp = {
        heap -> arr[b].value,
        heap -> arr[b].payload,
    };

    heap -> arr[b].value   = heap -> arr[a].value;
    heap -> arr[b].payload = heap -> arr[a].payload;
    heap -> arr[a].value   = tmp.value;
    heap -> arr[a].payload = tmp.payload;
}

size_t heap_size (Heap *heap)
{
    return heap -> arr[0].value;
}
