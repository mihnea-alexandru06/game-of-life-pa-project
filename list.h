#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "structures.h"

listnode_t* create_node(const int l, const int c);

void insert_sorted(listnode_t **head_ptr, const int l, const int c);

void free_list(listnode_t *head_ptr);

void print_list(FILE *out, listnode_t *head_ptr);

#endif