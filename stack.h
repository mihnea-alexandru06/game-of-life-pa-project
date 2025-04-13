#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

void print_list(FILE *out, listnode_t *head_ptr);

void free_list(listnode_t *head_ptr);

void push(stack_t *top_ptr, const unsigned int gen_k, listnode_t *list_changes);

void free_stack(stack_t top);

void print_stack_recursive(FILE *out, const stacknode_t *node);

void print_stack(FILE *out, const stack_t top);
#endif