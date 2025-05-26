#include "task3.h"
#include "tree.h"
#include <stdio.h>

static void handler(const treenode_t *ptr, grid_t *curr_grid, FILE *out) {
    if(ptr->gen_k > 0)
        apply_changes(curr_grid, ptr->changes);
    
    print_grid(out, curr_grid);
}

void solve_task3(FILE *out, const grid_t *init_grid, const int K) {
    tree_t tree = build_tree(init_grid, K);

    traverse_preorder(tree, init_grid, out, handler);
    
    free_tree(tree);
}