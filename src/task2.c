#include "task2.h"
#include "stack.h"
#include "list.h"
#include "rules.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

static int verify_reconstruction(const grid_t *final_grid, const grid_t *init_grid, stack_t top) {
    const int N = init_grid->rows;
    const int M = init_grid->cols;
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cell_state_t curr_state = (get_cell(final_grid, i, j) == STATE_ALIVE);
            
            stack_t curr = top;
            while (curr->prev != NULL) {
                if (in_list(curr->changes, i, j)) {
                    curr_state = flip_state(curr_state);
                }
                curr = curr->prev;
            }
            
            cell_state_t expected_state = get_cell(init_grid, i, j);
            if (curr_state != expected_state) {
                return 0;
            }
        }
    }
    
    return 1;
}

void solve_task2(FILE *out, const grid_t *init_grid, const int K)
{
    const int N = init_grid->rows, M = init_grid->cols;
    stack_t st = NULL;

    grid_t *final_grid = alloc_grid(N, M);
    copy_grid(final_grid, init_grid);

    listnode_t *init_changes = NULL, *init_tail = NULL;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (get_cell(init_grid, i, j) == STATE_ALIVE)
                insert_list(&init_changes, &init_tail, i, j);

    if (init_changes)
        push(&st, 0, init_changes);

    for (int gen = 1; gen <= K; ++gen) {
        listnode_t *std_changes = NULL, *std_tail = NULL;

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j) {
                int alive_now = is_alive(st, i, j);

                int alive_next = is_alive_next(st, i, j, N, M, alive_now, RULES_STD);

                if (alive_now != alive_next)
                    insert_list(&std_changes, &std_tail, i, j);
            }
        apply_changes(final_grid, std_changes);
        push(&st, gen, std_changes);
    }

    
    int bonus = verify_reconstruction(final_grid, init_grid, st);

    if(bonus)
        print_stack(out, st);
    free_stack(st);
    free_grid(final_grid);
}