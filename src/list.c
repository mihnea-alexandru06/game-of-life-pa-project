#include "list.h"
#include "structures.h"
#include <stdlib.h>
#include <stdio.h>

listnode_t* create_node(const int r, const int c) {
    listnode_t *ptr = malloc(sizeof(listnode_t));

    if(ptr == NULL)
        return NULL;
    
    ptr->coords.r = r;
    ptr->coords.c = c;
    ptr->next = NULL;

    return ptr;
}

void insert_list(listnode_t **head_ptr, listnode_t **tail_ptr, const int r, const int c) {
    listnode_t *ptr = create_node(r, c);
    if(ptr == NULL)
        return;
    
    ptr->next = NULL;
    if(*head_ptr == NULL) {
        *head_ptr = *tail_ptr = ptr;
    } else {
        (*tail_ptr)->next = ptr;
        *tail_ptr = ptr;
    }
}

void free_list(listnode_t *head_ptr) {
    listnode_t *ptr = head_ptr;
    listnode_t *next;

    while(ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
}

void print_list(FILE *out, listnode_t *head_ptr) {
    listnode_t *ptr = head_ptr;
    
    while(ptr != NULL) {
        fprintf(out, " %d %d", ptr->coords.r, ptr->coords.c);
        ptr = ptr->next;
    }

    fprintf(out, "\n");
}