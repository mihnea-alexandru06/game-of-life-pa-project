#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELL_ALIVE 'X'
#define CELL_DEAD '+'

//Macro pentru a inchide fisierele si a avansa
#define CLEANUP \
    do {                     \
        close_files(in, out);  \
        t -= 2;              \
    } while (0)

int check_file(const FILE *file) {
    //Daca fisierul este bun, returneaza 1, altfel, 0.
    return !(file == NULL);
}

//Numara vecinii unei celule
int count_neighbors(const char *grid, int N, int M, int x, int y) {
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
1. Orice celulă vie cu mai put,in de doi vecini în viat,ă moare (subpopulare).
2. Orice celulă vie cu doi sau trei vecini vii trăies,te în continuare.
3. Orice celulă vie cu mai mult de trei vecini vii moare (suprapopulare).
4. Orice celulă moartă cu exact trei vecini devine o celulă vie (reproducere).
*/
void compute_state(const char *curr_grid, char *next_grid, const int N, const int M) {

    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j) {
            int live_neighbors = count_neighbors(curr_grid, N, M, i, j);
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
            
            next_grid[i*M + j] = next_state;
    }
}

void close_files(FILE *in, FILE *out) {
    if(in) fclose(in);
    if(out) fclose(out);
}

void solve_task(FILE *out, const char *v, const int N, const int M, const int K) {
    //printf("Rezolv task pentru matrice %d x %d ...\n", N, M);

    //Creez doua "matrici"
    char *curr_grid = malloc(N * M * sizeof(char));
    char *next_grid = malloc(N * M * sizeof(char));

    //Verific daca s-au alocat corect
    if(!curr_grid || !next_grid) {
        fprintf(stderr, "Eroare");
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
int main(int argc, const char* argv[]) {
    
    if(argc < 3 || argc % 2 == 0) {
        printf("eroaREEEEEEE!!!");
        return 1;
    }

    int t = argc - 1;

    
    int T,N,M,K;

    //Rezolv fiecare task
    while(t) {

        //Extrag numele fisierelor
        const char *input_filename = argv[t-1];
        const char *output_filename = argv[t];

        //Deschid fisierele
        FILE *in = fopen(input_filename, "r");
        FILE *out = fopen(output_filename, "w");

        
        //Verific integritatea(? e termenul corect?) fisierelor
        if(!(check_file(in) && check_file(out))) {
            fprintf(stderr, "Eroare input/output: %s", argv[t]);

            //Inchid ce fisiere au fost deschise, t-=2...
            CLEANUP;
            continue;
        }

        //Verific & citesc T,N,M,K
        if(fscanf(in, "%d %d %d %d", &T, &N, &M, &K) != 4) {
            CLEANUP;
            continue; 
        }

        if(N <= 0 || M <= 0) {
            CLEANUP;
            continue;

        }

        //Aloc vectorul(! care este de fapt o matrice) dinamic & initializez
        char *v = malloc(M * N * sizeof(char));

        if(v == NULL) {
            CLEANUP;
            continue;
        }


        //Citesc matricea X+X
        int err = 0;
        for(int i = 0 ; i < N && !err; ++i) 
            for(int j = 0; j < M; ++j) 
                if(fscanf(in, " %c", &v[i*M + j]) != 1) {
                    err = 1;
                    break;
                }
            
        
        if(err) {
            free(v);
            CLEANUP;
            continue; 
        }
        
        
        //Am rezolvarea doar pentru T=1(care cred ca sunt taskurile 1-5)
        solve_task(out, v, N, M, K);

        //Dau free la vector
        free(v);

        //Inchid fisierele si avansez
        CLEANUP;
    }

    return 0;
}
