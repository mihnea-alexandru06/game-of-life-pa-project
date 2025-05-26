#include "grid.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

grid_t *alloc_grid(int rows, int cols) {
    grid_t *grid_ptr = malloc(sizeof(grid_t));

    if(grid_ptr == NULL)
        return NULL;
    
    grid_ptr->rows = rows;
    grid_ptr->cols = cols;
    grid_ptr->cells = malloc((size_t) rows * cols * sizeof(char));

    if((grid_ptr->cells) == NULL) {
        free(grid_ptr);
        return NULL;
    }

    return grid_ptr;
}

void free_grid(grid_t *grid_ptr) {
    if(grid_ptr == NULL)
        return;
    
    free(grid_ptr->cells);
    free(grid_ptr);
}

void copy_grid(grid_t *dst, const grid_t *src) {
    memcpy(dst->cells, src->cells, (size_t) src->rows * src->cols);
}

void print_grid(FILE *out, grid_t *grid_ptr) {
    const int N = grid_ptr->rows;
    const int M = grid_ptr->cols;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) 
            fputc(state_to_char(get_cell(grid_ptr, i, j)), out);
        fputc('\n', out);
    }

    fputc('\n', out);
}
void swap_grids(grid_t **a, grid_t **b) {
    grid_t *tmp = *a;
    *a = *b;
    *b = tmp;
}

cell_state_t get_cell(const grid_t *grid_ptr, const int row, const int col) {
    char raw = grid_ptr->cells[row * grid_ptr->cols + col];
    return char_to_state(raw);
}

void set_cell(grid_t *grid_ptr, const int row, const int col, const cell_state_t state) {
    grid_ptr->cells[row * grid_ptr->cols + col] = state_to_char(state);
}