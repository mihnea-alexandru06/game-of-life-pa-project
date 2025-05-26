#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "grid.h"
#include "cell_state.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

int check_file(const FILE *file);

void close_files(FILE *in, FILE *out);

int process_input(const char *input_filename, const char *output_filename);

#endif