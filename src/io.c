#include "io.h"
#include "grid.h"
#include "cell_state.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include <stdio.h>

int check_file(const FILE *file) {
    return !(file == NULL);
}

void close_files(FILE *in, FILE *out) {
    if(in) fclose(in);
    if(out) fclose(out);
}

int process_input(const char *input_filename, const char *output_filename) {
    FILE *in = NULL;
    FILE *out = NULL;

    grid_t *init_grid = NULL;

    int result = 0;

    in = fopen(input_filename, "r");
    out = fopen(output_filename, "w");

    if(!(check_file(in) && check_file(out)))
        goto cleanup;
    
    int T, N, M, K;
    if(fscanf(in, " %d %d %d %d", &T, &N, &M, &K) != 4)
        goto cleanup;

    if(N <= 0 || M <= 0)
        goto cleanup;

    init_grid = alloc_grid(N, M);

    if(init_grid == NULL)
        goto cleanup;
    
    int err = 0;
    for(int i = 0 ; i < N && !err; ++i) 
        for(int j = 0; j < M; ++j) {
            char chr;
            if(fscanf(in, " %c", &chr) != 1) {
                err = 1;
                break;
            }
            set_cell(init_grid, i, j, char_to_state(chr));
        }

    if(err)
        goto cleanup;
    

    if(T == 1) 
        solve_task1(out, init_grid, K);
    else if(T == 2)
        solve_task2(out, init_grid, K);
    else if(T == 3)
        solve_task3(out, init_grid, K);
    else if(T == 4)
        solve_task4(out, init_grid, K);
    
    result = 1;
    
    cleanup:
        if(!result)
            fprintf(stderr, "Eroare!\n");
        
        free_grid(init_grid);
        
        close_files(in, out);
        
        return result;

}