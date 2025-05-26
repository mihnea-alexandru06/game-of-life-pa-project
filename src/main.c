#include "../include/io.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc < 3 || argc % 2 == 0) {
        fprintf(stderr, "Eroare!\n");
        return EXIT_FAILURE;
    }

    int t = argc - 1;

    while(t) {
        const char *input_filename = argv[t-1];
        const char *output_filename = argv[t];

        if(process_input(input_filename, output_filename) == 0)
            fprintf(stderr, "Eroare!\n");
        
        t-=2;
    }

    return EXIT_SUCCESS;
}
