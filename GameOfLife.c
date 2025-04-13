#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "stack.h"
#include "list.h"

#define CELL_ALIVE 'X'
#define CELL_DEAD '+'

int check_file(const FILE *file) {
    //Daca fisierul este bun, returneaza 1, altfel, 0.
    return !(file == NULL);
}

//Numara vecinii unei celule
int count_neighbors(const char *grid, const int N, const int M, const int x, const int y) {
    int cnt = 0;

    //Iterez prin vecini si dau skip la celula
    for (int i = -1; i <= 1; ++i) {
        for(int j = -1; j <= 1; ++j) {
            if(i == 0 && j == 0) continue;
            

            //Initializez idx si idy(pozitia x si pozitia y a vecinului)
            int idx = x + i;
            int idy = y + j;

            //Verific daca nu ies din matrice
            if(idx >= 0 && idx < N && idy >= 0 && idy < M)
                if(grid[idx*M + idy] == CELL_ALIVE) ++cnt;
        }
    }
    return cnt;
}

/*
Reguli:
1. Orice celulă vie cu mai put,in de doi vecini în viață moare (subpopulare).
2. Orice celulă vie cu doi sau trei vecini vii trăiește în continuare.
3. Orice celulă vie cu mai mult de trei vecini vii moare (suprapopulare).
4. Orice celulă moartă cu exact trei vecini devine o celulă vie (reproducere).
*/
void compute_state(const char *curr_grid, char *next_grid, const int N, const int M) {
    //Parcurg matricea
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j) {
            int live_neighbors = count_neighbors(curr_grid, N, M, i, j); //Numar vecinii
            char curr_state = curr_grid[i*M + j];
            char next_state;

            if(curr_state == CELL_ALIVE) //Celula in viata
                if(live_neighbors == 2 || live_neighbors == 3)
                    next_state = CELL_ALIVE; //Regula 2
                else next_state = CELL_DEAD; //Altfel, suprapopulare sau subpopulare (Regula 1 sau 3)
            
            else //Celula moarta
                if(live_neighbors == 3)
                    next_state = CELL_ALIVE; //Reproducere (Regula 4)
                else next_state = CELL_DEAD; //Sta moarta
            
            next_grid[i*M + j] = next_state; //Actualizez starea
    }
}

void close_files(FILE *in, FILE *out) {
    if(in) fclose(in);
    if(out) fclose(out);
}

void solve_task1(FILE *out, const char *v, const int N, const int M, const int K) {
    //Creez doua "matrici"
    char *curr_grid = malloc(N * M * sizeof(char));
    char *next_grid = malloc(N * M * sizeof(char));

    //Verific daca s-au alocat corect
    if(!curr_grid || !next_grid) {
        fprintf(stderr, "Eroare!\n");
        free(curr_grid);
        free(next_grid);
        return;
    }

    memcpy(curr_grid, v, N * M * sizeof(char));

    //Iterez de la 0...K
    for(int itr = 0; itr <= K; ++itr) {

        //Afisez matricea
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) 
                fprintf(out, "%c", curr_grid[i * M + j]);
            fprintf(out, "\n");
        }

        fprintf(out, "\n");

        //Aplic regulile
        compute_state(curr_grid, next_grid, N, M);


        //Dau swap la "matrici"
        char *tmp = curr_grid;
        curr_grid = next_grid;
        next_grid = tmp;
        
    }

    //Dau free
    free(curr_grid);
    free(next_grid);
}


//TODO: Cand rezolv si task 3, ar trebui sa fac .c si .h dedicat pt astea, incep sa dau load la fisierul .c principal
void solve_task2(FILE *out, const char *v, const int N, const int M, const int K) {
    
    //Creez doua "matrici"
    char *curr_grid = malloc(N * M * sizeof(char));
    char *next_grid = malloc(N * M * sizeof(char));

    //Verific daca s-au alocat corect
    if(!curr_grid || !next_grid) {
        fprintf(stderr, "Eroare!\n");
        free(curr_grid);
        free(next_grid);
        return;
    }


    memcpy(curr_grid, v, N * M * sizeof(char));

    stack_t st = NULL;

    //Iterez de la 1...K (Pentru ca taskul 2 implica doar afisarea schimbarilor)
    for(int itr = 1; itr <= K; ++itr) {
        compute_state(curr_grid, next_grid, N, M);

        listnode_t *list_changes = NULL;

        //Adaug schimbarile intr-o lista
        for(int i = 0; i < N; ++i)
            for(int j = 0 ; j < M; ++j)
                if(curr_grid[i * M + j] != next_grid[i * M + j])
                    insert_sorted(&list_changes, i, j); //Ma folosesc de functia de insert, implementarea este in list.c
        
        push(&st, itr, list_changes); //Dau push la lista de schimbari pe stack, implementarea este in stack.c

        //Dau swap la "matrici"
        char *tmp = curr_grid;
        curr_grid = next_grid;
        next_grid = tmp;
    }

    //Dau print
    print_stack(out, st);

    //Dau free
    free(curr_grid);
    free(next_grid);
    free_stack(st);
}

int process_input(const char *input_filename, const char *output_filename) {
    //Initializez
    FILE *in = NULL;
    FILE *out = NULL;

    char *v = NULL;

    int result = 0;

    //Deschid fisierele
    in = fopen(input_filename, "r");
    out = fopen(output_filename, "w");

    //Daca unul dintre fisiere NU este ok
    if(!(check_file(in) && check_file(out)))
        goto cleanup;
    
    //Verific & citesc T,N,M,K
    int T, N, M, K;
    if(fscanf(in, " %d %d %d %d", &T, &N, &M, &K) != 4)
        goto cleanup; //Am scos macro-ul CLEANUP si l-am inlocuit cu varianta asta cu goto
                      //Mi se pare mai "curat"
    

    //Daca N sau N sunt invalide
    if(N <= 0 || M <= 0)
        goto cleanup;
    

    //Aloc memorie pentru matrice, care este sub forma de vector
    v = malloc(M * N * sizeof(char));

    //Verific daca am initializat bine
    if(v == NULL)
        goto cleanup;
    

    //Citesc matricea
    int err = 0;
    for(int i = 0 ; i < N && !err; ++i) 
        for(int j = 0; j < M; ++j) {
            if(fscanf(in, " %c", &v[i*M + j]) != 1) {
                err = 1;
                break;
            }
        }

    //Daca am gasit o problema la citirea matricii
    if(err)
        goto cleanup;
    

    //Aleg taskul
    if(T == 1) 
        solve_task1(out, v, N, M, K);
    else if(T == 2)
        solve_task2(out, v, N, M, K);
    
    result = 1;
    

    //Codul pentru cleanup. Labelul este rulat chiar si daca NU ma folosesc de "goto cleanup"!!
    cleanup:
        if(!result) //Daca il opresc fortat
            fprintf(stderr, "Eroare!\n");
        
        if(v)
            free(v);
        
        close_files(in, out);
        return result;

}


//Am modificat functia main astfel incat sa nu mai fie incarcata
int main(const int argc, const char* argv[]) {
    
    if(argc < 3 || argc % 2 == 0) {
        fprintf(stderr, "Eroare!\n");
        return 1;
    }

    //Initializez t
    int t = argc - 1;

    
    //Rezolv fiecare task
    while(t) {
        //Extrag numele fisierelor
        const char *input_filename = argv[t-1];
        const char *output_filename = argv[t];

        //Vad daca s-a intamplat o nenorocire
        if(process_input(input_filename, output_filename) == 0) //Rezolvarea testelor se face prin process_input
                                                                //care proceseaza inputul si apoi redirectioneaza spre functia corecta
            fprintf(stderr, "Eroare!\n");
        

        //Avansez
        t-=2;
    }

    return 0;
}
