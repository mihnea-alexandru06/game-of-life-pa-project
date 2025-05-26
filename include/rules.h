#ifndef RULES_H
#define RULES_H

#include <stdlib.h>
#include "grid.h"
#include "list.h"
#include "cell_state.h"
#include "structures.h"

typedef enum {
    RULES_STD,
    RULES_CUSTOM
}rules_t;

int in_list(const listnode_t *list, unsigned int r, unsigned int c);

int is_alive(stack_t st, unsigned int r, unsigned int c);

int count_neighbors(const grid_t *grid, int i, int j);

int count_neighbors_stack(const stack_t st, const int i, const int j, const int N, const int M);

void compute_state(const grid_t *curr_grid, grid_t *next_grid);

void compute_state_custom(const grid_t *curr_grid, grid_t *next_grid);

void apply_changes(grid_t *processed_grid, const listnode_t *changes);

int is_alive_next(const stack_t st,
                  int i, int j,
                  int N, int M,
                  int alive,
                  rules_t rules);


#endif