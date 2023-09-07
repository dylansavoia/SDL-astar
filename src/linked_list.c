#include "linked_list.h"
#include <stdlib.h>

LinkedList *LL_create ()
{
    LinkedList *new_ll  = malloc(sizeof(LinkedList));
    new_ll -> head = NULL;
    return new_ll;
}

void LL_destroy (LinkedList *ll)
{
    struct _ll_node *head = ll -> head;

    while (head != NULL)
    {
        struct _ll_node *next = head -> next;
        free(head);
        head = next;
    }

    free(ll);
}

void LL_push_front (LinkedList *ll, void *payload)
{
	struct _ll_node *new_node = malloc(sizeof(struct _ll_node));
	new_node -> payload = payload;
    new_node -> next    = ll -> head;
    ll -> head          = new_node;
}
