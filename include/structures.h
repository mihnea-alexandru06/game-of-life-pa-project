#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "../include/grid.h"

typedef struct {
    unsigned int r; /* Row */
    unsigned int c; /* Column */
}coords_t;

typedef struct listnode_t {
    coords_t coords;
    struct listnode_t *next;
}listnode_t;

typedef struct stacknode_t {
    unsigned int gen_k;
    listnode_t *changes;
    struct stacknode_t *prev; /* Pointer to the previous generation */
}stacknode_t;

typedef stacknode_t* stack_t;

typedef struct treenode_t {
    unsigned int gen_k;
    listnode_t *changes;
    struct treenode_t *left; /* Left child -> custom rule */
    struct treenode_t *right; /* Right child -> standard rule */
}treenode_t;

typedef treenode_t* tree_t;

#endif