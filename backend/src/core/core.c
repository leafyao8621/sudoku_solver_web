#include <stdbool.h>

#include <containers/dstring.h>

#include "core.h"

void advance(uint8_t **iter, uint8_t *end) {
    for (++(*iter); *iter < end && !(**iter & CELL_EMPTY); ++(*iter));
}

void retreat(uint8_t **iter, uint8_t *start) {
    for (--(*iter); *iter >= start && !(**iter & CELL_EMPTY); --(*iter));
}

bool increment(uint8_t *iter, uint8_t n) {
    if ((++(*iter) & ~CELL_EMPTY) > n) {
        *iter = CELL_EMPTY;
        return false;
    }
    return true;
}

bool check(
    uint8_t *grid,
    uint8_t row,
    uint8_t column,
    uint8_t block_row,
    uint8_t block_column,
    uint8_t n,
    String *lookup) {
    memset(lookup->data, 0, n);
    uint8_t *iter = grid + row * n;
    uint8_t val;
    for (uint8_t i = 0; i < n; ++i, ++iter) {
        val = (*iter) & ~CELL_EMPTY;
        if (val && lookup->data[val - 1]) {
            return false;
        } else if (val) {
            lookup->data[val - 1] = 1;
        }
    }
    memset(lookup->data, 0, n);
    iter = grid + column;
    for (uint8_t i = 0; i < n; ++i, iter += n) {
        val = (*iter) & ~CELL_EMPTY;
        if (val && lookup->data[val - 1]) {
            return false;
        } else if (val) {
            lookup->data[val - 1] = 1;
        }
    }
    memset(lookup->data, 0, n);
    iter =
        grid +
        (row - (row % block_row)) * n +
        (column - (column % block_column));
    for (uint8_t i = 0; i < block_row; ++i, iter += n - block_column) {
        for (uint8_t j = 0; j < block_column; ++j, ++iter) {
            val = (*iter) & ~CELL_EMPTY;
            if (val && lookup->data[val - 1]) {
                return false;
            } else if (val) {
                lookup->data[val - 1] = 1;
            }
        }
    }
    return true;
}

int solve(uint8_t *grid, uint8_t block_row, uint8_t block_column) {
    if (!grid) {
        return STATUS_ERROR;
    }
    if (block_row > 11 || block_column > 11) {
        return STATUS_ERROR;
    }
    uint16_t idx, size;
    uint8_t n, row, column;
    n = block_row * block_column;
    size = n * n;
    String lookup;
    int ret = DArrayChar_initialize(&lookup, n);
    if (ret) {
        return STATUS_ERROR;
    }
    uint8_t *iter = grid;
    uint8_t *end = grid + size;
    if (!(*iter & CELL_EMPTY)) {
        advance(&iter, end);
    }
    for (; iter >= grid && iter < end;) {
        idx = iter - grid;
        row = idx / n;
        column = idx % n;
        for (; increment(iter, n);) {
            if (check(grid, row, column, block_row, block_column, n, &lookup)) {
                break;
            }
        }
        if (*iter & ~CELL_EMPTY) {
            advance(&iter, end);
        } else {
            retreat(&iter, grid);
        }
    }
    DArrayChar_finalize(&lookup);
    if (iter == grid - 1) {
        return STATUS_NO_SOLUTION;
    }
    return STATUS_SOLUTION_FOUND;
}

int grid_log(uint8_t *grid, uint8_t block_row, uint8_t block_column, FILE *fout) {
    if (!grid || !fout) {
        return 1;
    }
    uint8_t *iter = grid;
    uint8_t n = block_row * block_column;
    for (uint8_t i = 0; i < n; ++i) {
        for (uint8_t j = 0; j < n; ++j, ++iter) {
            fprintf(fout, "%hhu ", *iter & ~CELL_EMPTY);
        }
        fputc(10, fout);
    }
    return 0;
}
