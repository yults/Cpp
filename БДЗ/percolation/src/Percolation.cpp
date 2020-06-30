#include "Percolation.h"
#include <vector>

const std::vector <int> move = {1, 0, -1, 0, 0, 1, 0, -1};

Percolation::Percolation(size_t dimension) : dim(dimension), opencells(0)
{
    std::vector<STATE> fil(dim, BLOCK);
    for (size_t i = 0; i < dim; i++) {
        cell.push_back(fil);
    }
}

bool Percolation::is_index(size_t row, size_t column)
{
    return (row < dim && column < dim);
}

void Percolation::make_full(size_t row, size_t column)
{
    cell[row][column] = FULL;
    for (int i = 0; i < 4; i++) {
        if (!((row == 0 && i == 2) || (column == 0 && i == 3))) {
            size_t row_new = row + move[i];
            size_t column_new = column + move[i + 4];
            if (is_index(row_new, column_new) && is_open(row_new, column_new) && !is_full(row_new, column_new)) {
                make_full(row_new, column_new);
            }
        }
    }
}

void Percolation::open(size_t row, size_t column) {
    if (is_open(row, column)) return;
    opencells++;
    cell[row][column] = OPEN;
    bool need_make_ful = false;
    for (int i = 0; i < 4; i++) {
        if (!((row == 0 && i == 2) || (column == 0 && i == 3))) {
            size_t row_new = row + move[i];
            size_t column_new = column + move[i + 4];
            if (is_index(row_new, column_new) && is_full(row_new, column_new)) {
                need_make_ful = true;
            }
        }
    }
    if (need_make_ful || row == 0) {
        make_full(row, column);
    }
}

bool Percolation::is_open(size_t row, size_t column) const
{
    return cell[row][column] != BLOCK;
}

bool Percolation::is_full(size_t row, size_t column) const
{
    return cell[row][column] == FULL;
}

size_t Percolation::get_numbet_of_open_cells() const
{
    return opencells;
}

bool Percolation::has_percolation() const {
    for (size_t i = 0; i < dim; i++) {
        if (is_full(dim - 1, i)) return true;
    }
    return false;
}

