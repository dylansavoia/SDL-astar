#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct _ll_node {
    struct _ll_node *next;
    void *payload;
};

typedef struct LinkedList {
    struct _ll_node *head;
} LinkedList;

LinkedList *LL_create ();
void LL_destroy (LinkedList *ll);

void LL_push_front (LinkedList *ll, void *payload);

#endif
