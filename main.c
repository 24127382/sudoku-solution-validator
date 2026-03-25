#include "validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern int board[9][9];
extern int result[27];

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <sudoku_file.txt>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    // Read the sudoku board from the text file
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fscanf(file, "%d", &board[i][j]) != 1) {
                printf("Error: Invalid board format\n");
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);

    printf("Sudoku Board:\n");
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i != 0) printf("------+-------+------\n");
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 && j != 0) printf("| ");
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Initialize result array
    memset(result, 0, sizeof(result));

    // Create threads for validation
    pthread_t threads[27];
    int thread_index = 0;

    // Validate rows (0-8)
    for (int i = 0; i < 9; i++) {
        parameters* args = malloc(sizeof(parameters));
        args->row = i;
        args->col = 0;
        args->thread_id = thread_index++;
        pthread_create(&threads[args->thread_id], NULL, validate_row, args);
    }

    // Validate columns (9-17)
    for (int j = 0; j < 9; j++) {
        parameters* args = malloc(sizeof(parameters));
        args->row = 0;
        args->col = j;
        args->thread_id = thread_index++;
        pthread_create(&threads[args->thread_id], NULL, validate_column, args);
    }

    // Validate 3x3 subgrids (18-26)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            parameters* args = malloc(sizeof(parameters));
            args->row = i * 3;
            args->col = j * 3;
            args->thread_id = thread_index++;
            pthread_create(&threads[args->thread_id], NULL, validate_subgrid, args);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    int valid = 1;
    for (int i = 0; i < 27; i++) {
        if (result[i] == 0) {
            valid = 0;
            break;
        }
    }

    printf("Validation Result: %s\n", valid ? "VALID Sudoku" : "INVALID Sudoku");
    return valid ? 0 : 1;
}
