#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    int l; //linie
    int c; //coloana
}coords_t;

typedef struct listnode_t {
    coords_t coords;
    struct listnode_t *next;
}listnode_t;

typedef struct stacknode_t {
    unsigned int gen_k; //Generatia
    listnode_t *changes;
    struct stacknode_t *prev; //Pointer la generatia anterioara
}stacknode_t;

typedef stacknode_t* stack_t;

#endif