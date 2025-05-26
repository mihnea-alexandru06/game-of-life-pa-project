#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "grid.h"
#include "structures.h"

typedef struct {
   unsigned int nodes;
   unsigned int edges; 
   unsigned int *degree;
   unsigned int *adj;
}graph_t;

typedef struct {
    unsigned int length;
    coords_t *coords;
}path_t;

graph_t *create_graph(unsigned int nodes);

graph_t *build_graph(const grid_t *grid_ptr);

void free_graph(graph_t *graph_ptr);

void graph_add(graph_t *graph_ptr, unsigned int x, unsigned int y);

path_t longest_hamiltonian(const graph_t *graph_ptr);

void free_path(path_t *path_ptr);

void print_path(FILE *out, path_t *path_ptr);

#endif

