#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

listnode_t* create_node(const int r, const int c);

void insert_list(listnode_t **head_ptr, listnode_t **tail_ptr, const int r, const int c);

void free_list(listnode_t *head_ptr);

void print_list(FILE *out, listnode_t *head_ptr);

#endif