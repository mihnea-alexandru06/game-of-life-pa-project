#include "task4.h"
#include "tree.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

static void handler(const treenode_t *ptr, grid_t *curr_grid, FILE *out) {
    if(ptr->gen_k > 0)
        apply_changes(curr_grid, ptr->changes);

    graph_t *g = build_graph(curr_grid);
    path_t p   = longest_hamiltonian(g);

    print_path(out, &p);

    free_path(&p);
    free_graph(g);
}


void solve_task4(FILE *out, const grid_t *init_grid, const int K) {
    tree_t tree = build_tree(init_grid, K);

    traverse_preorder(tree, init_grid, out, handler);
    
    free_tree(tree);
}