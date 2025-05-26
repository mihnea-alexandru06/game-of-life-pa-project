#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "rules.h"
#include "cell_state.h"
#include "structures.h"
#include "grid.h"


typedef void (*traversal_handler_t)(const treenode_t *ptr, grid_t *curr_grid, FILE *out);

treenode_t* create_treenode(unsigned int gen_k, listnode_t *changes);

tree_t build_tree(const grid_t *init_grid, int K);

void traverse_preorder(const tree_t root, const grid_t *init_grid, FILE *out, traversal_handler_t handler);

void free_tree(tree_t root);

#endif