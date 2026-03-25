#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <pthread.h>
#include <stdlib.h>

void* validate_row(void* param);
void* validate_column(void* param);
void* validate_subgrid(void* param);

typedef struct {
    int row;
    int col;
    int thread_id;
} parameters;

extern int board[9][9];

// store the result after validating each row, column and 3x3 subgrid
extern int result[27];

#endif
