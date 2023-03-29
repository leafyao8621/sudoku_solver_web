#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>
#include <stdint.h>

#define CELL_EMPTY 0x80

#define STATUS_SOLUTION_FOUND 0
#define STATUS_NO_SOLUTION 1
#define STATUS_ERROR 2

int solve(uint8_t *grid, uint8_t block_row, uint8_t block_column);
int grid_log(uint8_t *grid, uint8_t block_row, uint8_t block_column, FILE *fout);

#endif
