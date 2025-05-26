#include "graph.h"
#include <stdlib.h>
#include <limits.h>

#define ADJ(g,i,j) ((g)->adj[(size_t)(i) * (g)->nodes + (j)])


static coords_t *node_coords = NULL;

static void free_coords(void) {
    free(node_coords);
    node_coords = NULL;
}

graph_t *create_graph(unsigned int nodes) {
    graph_t *graph_ptr = malloc(sizeof(graph_t));

    if(graph_ptr == NULL)
        return NULL;

    graph_ptr->nodes  = nodes;
    graph_ptr->edges  = 0;
    graph_ptr->degree = calloc(nodes, sizeof(unsigned int));
    graph_ptr->adj    = calloc((size_t) nodes * nodes, sizeof(unsigned int));

    if(graph_ptr->degree == NULL || graph_ptr->adj == NULL) {
        free(graph_ptr->degree);
        free(graph_ptr->adj);
        free(graph_ptr);
        return NULL;
    }

    return graph_ptr;
}

void graph_add(graph_t *graph_ptr, unsigned int x, unsigned int y) {
    if(!ADJ(graph_ptr, x, y)) {
        ADJ(graph_ptr, x, y) = ADJ(graph_ptr, y, x) = 1;
        graph_ptr->degree[x]++;
        graph_ptr->degree[y]++;
        graph_ptr->edges++;
    }
}

void free_graph(graph_t *graph_ptr) {
    if(graph_ptr == NULL)
        return;

    free(graph_ptr->adj);
    free(graph_ptr->degree);
    free(graph_ptr);

    free_coords();
}

graph_t *build_graph(const grid_t *grid_ptr) {
    const int N = grid_ptr->rows;
    const int M = grid_ptr->cols;

    unsigned int *id = malloc((size_t) N * M * sizeof(unsigned int));
    if(id == NULL)
        return NULL;

    unsigned int cnt = 0;
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j)
            if(get_cell(grid_ptr, i, j) == STATE_ALIVE)
                id[i * M + j] = cnt++;
            else
                id[i * M + j] = UINT_MAX;

    graph_t *graph_ptr = create_graph(cnt);
    if(graph_ptr == NULL) {
        free(id);
        return NULL;
    }

    node_coords = malloc(cnt * sizeof(coords_t));
    if(node_coords == NULL) {
        free_graph(graph_ptr);
        free(id);
        return NULL;
    }

    /* Fill adjacency. For each live cell, check all its neighbors. */
    unsigned int idx = 0;
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j)
            if(get_cell(grid_ptr, i, j) == STATE_ALIVE) {
                node_coords[idx].r = (unsigned) i;
                node_coords[idx].c = (unsigned) j;

                for (int di = -1; di <= 1; ++di) {
                    for(int dj = -1; dj <= 1; ++dj) {
                        if(di == 0 && dj == 0) continue;
                        

                        int next_r = i + di;
                        int next_c = j + dj;

                        if(next_r >= 0 && next_r < N && next_c >= 0 && next_c < M)
                            if(get_cell(grid_ptr, next_r, next_c) == STATE_ALIVE) {
                               unsigned int v = id[next_r * M + next_c];

                               if(v > idx)
                                    graph_add(graph_ptr, idx, v);
                            }
                    }
                }

                ++idx;
            }

    free(id);
    return graph_ptr;
}

static int coords_less(unsigned int i, unsigned int j) {
    coords_t a = node_coords[i];
    coords_t b = node_coords[j];

    if(a.r != b.r)
        return a.r < b.r;
    return a.c < b.c;
}

/* DFS for all nodes of a conex component */
static void dfs_component(const graph_t *graph_ptr, unsigned int v, unsigned char *vis, unsigned int *list, unsigned int *cnt) {
    vis[v] = 1;
    list[(*cnt)++] = v;

    for(unsigned int u = 0; u < graph_ptr->nodes; ++u)
        if(ADJ(graph_ptr, v, u) && !vis[u])
            dfs_component(graph_ptr, u, vis, list, cnt);
}

static void sort_indices(unsigned int *list, unsigned int cnt) {
    for(unsigned int i = 0; i < cnt; ++i)
        for(unsigned int j = i + 1; j < cnt; ++j)
            if(!coords_less(list[i], list[j])) {
                unsigned int tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
}

/* DFS backtracking for searching Hamiltonian Path */
static int dfs_hamilton(const graph_t *graph_ptr, unsigned long long vis_mask, unsigned int depth, unsigned int total, unsigned int *curr, unsigned int *best) {
    if(depth == total) {
        for(unsigned int i = 0; i < total; ++i)
            best[i] = curr[i];
        return 1; /* Perfect Hamiltonian Path Found */
    }

    unsigned int v = curr[depth - 1];
    unsigned int neigh[128], neigh_cnt = 0;

    for(unsigned int u = 0; u < graph_ptr->nodes; ++u)
        if(ADJ(graph_ptr, v, u) && !(vis_mask & (1ULL << u)))
            neigh[neigh_cnt++] = u;

    /* Sorting neighbors lexicographically */
    for(unsigned int i = 0; i < neigh_cnt; ++i)
        for(unsigned int j = i + 1; j < neigh_cnt; ++j)
            if(!coords_less(neigh[i], neigh[j])) {
                unsigned int tmp = neigh[i];
                neigh[i] = neigh[j];
                neigh[j] = tmp;
            }

    for(unsigned int k = 0; k < neigh_cnt; ++k) {
        unsigned int u = neigh[k];
        curr[depth] = u;
        if(dfs_hamilton(graph_ptr, vis_mask | (1ULL << u), depth + 1, total, curr, best))
            return 1; /* Early exit */
    }

    return 0;
}

/**
 * Find the longest Hamiltonian Path acros all components.
 * Break ties by lexicographically smaller coordinate sequences.
 */
path_t longest_hamiltonian(const graph_t *graph_ptr) {
    path_t path = {0, NULL};

    if(graph_ptr == NULL || graph_ptr->nodes == 0)
        return path;

    const unsigned int N = graph_ptr->nodes;

    unsigned char *vis = calloc(N, 1);
    unsigned int *comp = malloc(N * sizeof(unsigned int));
    unsigned int *best_chain = NULL;
    unsigned int best_sz = 0;

    if(vis == NULL || comp == NULL) {
        free(vis);
        free(comp);
        return path;
    }

    /* Explore each component via DFS */
    for(unsigned int i = 0; i < N; ++i) {
        if(!vis[i]) {
            unsigned int cnt = 0;
            dfs_component(graph_ptr, i, vis, comp, &cnt);

            if(cnt == 0)
                continue;

            sort_indices(comp, cnt);

            /* Try to find a Hamiltonian path starting at each node */
            unsigned int *curr  = malloc(cnt * sizeof(unsigned int));
            unsigned int *chain = malloc(cnt * sizeof(unsigned int));

            if(curr == NULL || chain == NULL) {
                free(curr);
                free(chain);
                continue;
            }

            int found = 0;
            for(unsigned int j = 0; j < cnt && !found; ++j) {
                curr[0] = comp[j];
                if(dfs_hamilton(graph_ptr, 1ULL << comp[j], 1, cnt, curr, chain))
                    found = 1;
            }

            /* If found, compare to best (prefer larger or lexicographically smaller) */
            if(found) {
                if(cnt > best_sz) {
                    best_sz = cnt;
                    free(best_chain);
                    best_chain = malloc(cnt * sizeof(unsigned int));
                    for(unsigned int k = 0; k < cnt; ++k)
                        best_chain[k] = chain[k];
                } else if(cnt == best_sz) {
                    for(unsigned int k = 0; k < cnt; ++k) {
                        coords_t a = node_coords[chain[k]];
                        coords_t b = node_coords[best_chain[k]];

                        if(a.r < b.r || (a.r == b.r && a.c < b.c)) {
                            for(unsigned int m = 0; m < cnt; ++m)
                                best_chain[m] = chain[m];
                            break;
                        } else if(a.r > b.r || (a.r == b.r && a.c > b.c)) {
                            break;
                        }
                    }
                }
            }

            free(curr);
            free(chain);
        }
    }

    free(vis);
    free(comp);

    if(best_sz == 0)
        return path; /* No Path Found */

    /* Build Final Path from best_chain */
    path.length = best_sz - 1;
    path.coords = malloc(best_sz * sizeof(coords_t));

    if(path.coords == NULL) {
        path.length = 0;
        return path;
    }

    for(unsigned int i = 0; i < best_sz; ++i)
        path.coords[i] = node_coords[best_chain[i]];

    free(best_chain);
    return path;
}

void print_path(FILE *out, path_t *path_ptr) {
    if(path_ptr == NULL || path_ptr->coords == NULL) {
        fprintf(out, "-1\n");
        return;
    }

    fprintf(out, "%u\n", path_ptr->length);

    for(unsigned int i = 0; i <= path_ptr->length; ++i) {
        fprintf(out, "(%u,%u)%c", path_ptr->coords[i].r, path_ptr->coords[i].c,
                (i == path_ptr->length ? '\n' : ' '));
    }
}

void free_path(path_t *path_ptr) {
    if(path_ptr == NULL || path_ptr->coords == NULL)
        return;

    free(path_ptr->coords);
    path_ptr->coords = NULL;
    path_ptr->length = 0;
}
