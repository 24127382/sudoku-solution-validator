# include "validator.h"

int board[9][9];
int result[27];

void* validate_row(void* param) {
    parameters* args = (parameters*) param;

    int freq[10] = {0}; // frequency array to count occurrences of numbers 1-9
    for (int c = 0; c < 9; c++) {
        if (freq[board[args->row][c]] == 1) {
            result[args->thread_id] = 0; // duplicate found, invalid row
            return NULL;
        }else{
            freq[board[args->row][c]] = 1; // mark the number as seen
        }
    }
    result[args->thread_id] = 1; // row is valid
    free(args);
    return NULL;
}

void* validate_column(void* param) {
    parameters* args = (parameters*) param;

    int freq[10] = {0}; // frequency array to count occurrences of numbers 1-9
    for (int r = 0; r < 9; r++) {
        if (freq[board[r][args->col]] == 1) {
            result[args->thread_id] = 0; // duplicate found, invalid column
            return NULL;
        }else{
            freq[board[r][args->col]] = 1; // mark the number as seen
        }
    }
    result[args->thread_id] = 1; // column is valid
    free(args);
    return NULL;
}

void* validate_subgrid(void* param) {
    parameters* args = (parameters*) param;
    int startRow = (args->row / 3) * 3; // calculate starting row of the 3x3 subgrid
    int startCol = (args->col / 3) * 3; // calculate starting column of the 3x3 subgrid

    int freq[10] = {0}; // frequency array to count occurrences of numbers 1-9
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (freq[board[r][c]] == 1) {
                result[args->thread_id] = 0; // duplicate found, invalid subgrid
                return NULL;
            }else{
                freq[board[r][c]] = 1; // mark the number as seen
            }
        }
    }
    result[args->thread_id] = 1; 
    free(args); 
    return NULL;
}