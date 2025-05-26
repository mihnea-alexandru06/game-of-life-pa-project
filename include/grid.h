#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cell_state.h"

typedef struct {
    int rows, cols;
    char *cells;
}grid_t;

grid_t *alloc_grid(int rows, int cols);

void free_grid(grid_t *grid_ptr);

void copy_grid(grid_t *dst, const grid_t *src);

void print_grid(FILE *out, grid_t *grid_ptr);

void swap_grids(grid_t **a, grid_t **b);

cell_state_t get_cell(const grid_t *grid_ptr, const int row, const int col);

void set_cell(grid_t *grid_ptr, const int row, const int col, const cell_state_t state);

#endif