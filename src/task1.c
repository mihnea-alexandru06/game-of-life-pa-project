#include "task1.h"
#include "grid.h"
#include "cell_state.h"
#include "rules.h"
#include <stdio.h>
#include <stdlib.h>

void solve_task1(FILE *out, const grid_t *init_grid, const int K) {
    const int N = init_grid->rows;
    const int M = init_grid->cols;

    grid_t *curr_grid = alloc_grid(N, M);
    grid_t *next_grid = alloc_grid(N, M);

    if(!curr_grid || !next_grid) {
        fprintf(stderr, "Eroare!\n");
        free_grid(curr_grid);
        free_grid(next_grid);
        return;
    }

    copy_grid(curr_grid, init_grid);

    for(int itr = 0; itr <= K; ++itr) {

        print_grid(out, curr_grid);

        compute_state(curr_grid, next_grid);

        swap_grids(&curr_grid, &next_grid);
    }

    free_grid(curr_grid);
    free_grid(next_grid);
}