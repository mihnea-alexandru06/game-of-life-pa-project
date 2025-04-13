#include "structures.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void push(stack_t *top_ptr, const unsigned int gen_k, listnode_t *list_changes) {
    stacknode_t *ptr = malloc(sizeof(stacknode_t));
    if(ptr == NULL)
        return; //Ar trebui sa fac si error handling
    
    //Initializez
    ptr->gen_k = gen_k;
    ptr->changes = list_changes;
    ptr->prev = *top_ptr;
    *top_ptr = ptr;
}

void free_stack(stack_t top) {
    stacknode_t *curr = top;
    stacknode_t *prev;

    //Cat timp exista stacknode_t urmator(Ma duc de la generatia K+1, la generatia K!!!!!)
    while(curr != NULL) {
        prev = curr->prev;
        free_list(curr->changes);
        free(curr);
        curr = prev;
    }
}

void print_stack_recursive(FILE *out, const stacknode_t *node) {
    if(node == NULL)
        return;
    
    //Ma duc mai in adancime inainte sa afisez
    print_stack_recursive(out, node->prev);

    //Afisez generatia, apoi afisez schimbarile
    fprintf(out, "%d", node->gen_k);
    print_list(out, node->changes);
}

void print_stack(FILE *out, const stack_t top) {
    print_stack_recursive(out, top);
}