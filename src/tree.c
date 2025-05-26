#include "tree.h"
#include "stack.h"
#include "list.h"
#include "cell_state.h"
#include "rules.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

treenode_t* create_treenode(unsigned int gen_k, listnode_t *changes) {
    treenode_t* ptr = malloc(sizeof(treenode_t));
    if(ptr == NULL)
        return NULL;
    
    ptr->gen_k = gen_k;
    ptr->changes = changes;
    ptr->left = ptr->right = NULL;

    return ptr;
}

static void build_children(treenode_t *ptr, const grid_t *init_grid, stack_t parent_stack, int gen_k, int K) {
    if(gen_k >= K)
        return;
    
    const int N = init_grid->rows;
    const int M = init_grid->cols;

    listnode_t *std_changes = NULL, *std_tail = NULL;
    listnode_t *custom_changes = NULL, *cust_tail = NULL;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int alive_now = is_alive(parent_stack, i, j);

            int alive_next_std = is_alive_next(parent_stack, i, j, N, M, alive_now, RULES_STD);

            int alive_next_custom = is_alive_next(parent_stack, i, j, N, M, alive_now, RULES_CUSTOM);

            if (alive_now != alive_next_std)
                insert_list(&std_changes, &std_tail, i, j);
            
            if (alive_now != alive_next_custom)
                insert_list(&custom_changes, &cust_tail, i, j);
        }
    }

    ptr->left = create_treenode(gen_k + 1, custom_changes);
    ptr->right = create_treenode(gen_k + 1, std_changes);

    stack_t left_stack = parent_stack;
    stack_t right_stack = parent_stack;
    
    if (custom_changes != NULL) {
        push(&left_stack, gen_k + 1, custom_changes);
    }
    if (std_changes != NULL) {
        push(&right_stack, gen_k + 1, std_changes);
    }

    build_children(ptr->left, init_grid, left_stack, gen_k + 1, K);
    build_children(ptr->right, init_grid, right_stack, gen_k + 1, K);

    if (custom_changes != NULL && left_stack != parent_stack) {
        stacknode_t *temp = left_stack;
        left_stack = left_stack->prev;
        temp->prev = NULL;
        free(temp);
    }
    if (std_changes != NULL && right_stack != parent_stack) {
        stacknode_t *temp = right_stack;
        right_stack = right_stack->prev;
        temp->prev = NULL;
        free(temp);
    }
}

tree_t build_tree(const grid_t *init_grid, int K) {
    const int N = init_grid->rows;
    const int M = init_grid->cols;

    listnode_t *root_changes = NULL, *root_changes_tail = NULL;

    stack_t root_stack = NULL;

    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j)
            if(get_cell(init_grid, i, j) == STATE_ALIVE)
                insert_list(&root_changes, &root_changes_tail, i, j);
    
    treenode_t *root = create_treenode(0, root_changes);
    
    push(&root_stack, 0, root_changes);

    build_children(root, init_grid, root_stack, 0, K);

    free(root_stack);

    return root;
}

static void traverse_node(const treenode_t *ptr, grid_t *curr_grid, FILE *out, traversal_handler_t handler) {
    if(ptr == NULL)
        return;
    
    const int N = curr_grid->rows;
    const int M = curr_grid->cols;

    /* Calling the handler */
    handler(ptr, curr_grid, out);

    if(ptr->left == NULL || ptr->right == NULL)
        return;
    
    grid_t *aux_grid = alloc_grid(N, M);

    copy_grid(aux_grid, curr_grid); 
    traverse_node(ptr->left, aux_grid, out, handler);

    copy_grid(aux_grid, curr_grid);
    traverse_node(ptr->right, aux_grid, out, handler);

    free_grid(aux_grid);


}

void traverse_preorder(const tree_t root, const grid_t *init_grid, FILE *out, traversal_handler_t handler) {
    const int N = init_grid->rows;
    const int M = init_grid->cols;

    grid_t *curr_grid = alloc_grid(N, M);

    copy_grid(curr_grid, init_grid);

    traverse_node(root, curr_grid, out, handler);

    free_grid(curr_grid);
}

void free_tree(tree_t root) {
    if(root == NULL)
        return;
    
    free_tree(root->left);
    free_tree(root->right);

    free_list(root->changes);

    free(root);
}