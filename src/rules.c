#include "rules.h"
#include <stdlib.h>

int in_list(const listnode_t *list, unsigned int r, unsigned int c) {
    for ( ; list ; list = list->next)
        if (list->coords.r == r && list->coords.c == c)
            return 1;
    return 0;
}

int is_alive(stack_t st, unsigned int r, unsigned int c) {
    int parity = 0;

    for ( ; st ; st = st->prev)
        if (in_list(st->changes, r, c))
            parity ^= 1;              
    return parity;                  
}

int count_neighbors(const grid_t *grid, const int i, const int j) {
    const int N = grid->rows;
    const int M = grid->cols;

    int cnt = 0;

    for (int di = -1; di <= 1; ++di) {
        for(int dj = -1; dj <= 1; ++dj) {
            if(di == 0 && dj == 0) continue;
            

            int next_r = i + di;
            int next_c = j + dj;

            if(next_r >= 0 && next_r < N && next_c >= 0 && next_c < M)
                if(get_cell(grid, next_r, next_c) == STATE_ALIVE) 
                    ++cnt;
        }
    }
    return cnt;
}

int count_neighbors_stack(const stack_t st, const int i, const int j, const int N, const int M) {
    int cnt = 0;

    for (int di = -1; di <= 1; ++di) {
        for(int dj = -1; dj <= 1; ++dj) {
            if(di == 0 && dj == 0) continue;

            int next_r = i + di;
            int next_c = j + dj;

            if(next_r >= 0 && next_r < N && next_c >= 0 && next_c < M)
                cnt += is_alive(st, next_r, next_c);
            
        }
    }
    return cnt;
}

int is_alive_next(const stack_t st, const int i, const int j, const int N, const int M, const int alive, rules_t rules) {
    int live_neighbors = count_neighbors_stack(st, i, j, N, M);

    int alive_next = alive;

    if(rules == RULES_STD) {
        if(alive)
            alive_next = (live_neighbors == 2 || live_neighbors == 3);
        else
            alive_next = live_neighbors == 3;
    } else {
        alive_next = (live_neighbors == 2) ? 1 : alive;
    }
    
    return alive_next;
}

void compute_state(const grid_t *curr_grid, grid_t *next_grid) {
    const int N = curr_grid->rows;
    const int M = curr_grid->cols;

    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j) {
            int live_neighbors = count_neighbors(curr_grid, i, j);
            cell_state_t curr_state = get_cell(curr_grid, i, j);
            cell_state_t next_state;

            if(curr_state == STATE_ALIVE)
                if(live_neighbors == 2 || live_neighbors == 3)
                    next_state = STATE_ALIVE;
                else next_state = STATE_DEAD;
            
            else
                if(live_neighbors == 3)
                    next_state = STATE_ALIVE;
                else next_state = STATE_DEAD;
            
            set_cell(next_grid, i, j, next_state);
    }
}

void compute_state_custom(const grid_t *curr_grid, grid_t *next_grid) {
    const int N = curr_grid->rows;
    const int M = curr_grid->cols;

    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j) {
            int live_neighbors = count_neighbors(curr_grid, i, j);
            cell_state_t curr_state  = get_cell(curr_grid, i, j);
            cell_state_t next_state = curr_state;

            if(live_neighbors == 2)
                next_state = STATE_ALIVE;

            set_cell(next_grid, i, j, next_state);
    }
}

void apply_changes(grid_t *processed_grid, const listnode_t *changes) {
    const listnode_t *curr = changes;
    
    while(curr != NULL) {
        int i = curr->coords.r;
        int j = curr->coords.c;

        cell_state_t old = get_cell(processed_grid, i, j);
        set_cell(processed_grid, i, j, flip_state(old));

        curr = curr->next;
    }
}