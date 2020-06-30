#include "board.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <utility>

Board Board::create_goal(const unsigned size)
{
    std::vector<std::vector<unsigned>> temp(size, std::vector<unsigned> (size));
    for (unsigned i = 0; i < size; i++)
        for (unsigned j = 0; j < size; j++)
            temp[i][j] = size * i + j + 1;
    temp[size - 1][size - 1] = 0;
    return Board(temp);
}

Board::Board()
{
    my_size = 0;
    my_board.push_back({0});
};

Board::Board(const unsigned size)
{
    my_size = size;
    if (my_size == 0 || my_size == 1)
    {
        my_board.push_back({0});
        return;
    }
    my_board.resize(size, std::vector<unsigned>(size));
    std::vector<unsigned> new_perm(size * size);
    for (unsigned i = 0; i < size * size; i++)
        new_perm[i] = i;
    std::shuffle(new_perm.begin(), new_perm.end(), std::mt19937(std::random_device()()));
    for (unsigned i = 0; i < size; i++)
    {
        for (unsigned j = 0; j < size; j++)
        {
            int temp = new_perm[size * i + j];
            my_board[i][j] = temp;
            find_man_ham(i, j, temp);
        }
    }
}

Board::Board(std::vector<std::vector<unsigned>> data) : my_board(std::move(data))
{
    if (data.empty()) my_size = 0;
    my_size = my_board.size();
    if (my_size == 0 || my_size == 1)
    {
        return;
    }
    for (size_t i = 0; i < my_size; i++)
    {
        for (size_t j = 0; j < my_size; j++)
        {
            int temp = my_board[i][j];
            find_man_ham(i, j, temp);
        }
    }
}

void Board::find_man_ham(unsigned i, unsigned j, int temp)
{
    if (temp == 0)
    {
        not_occupied = std::make_pair(i, j);
        if (my_size * my_size != my_size * i + j + 1) my_hamming++;
        return;
    }
    if (temp != (int)(my_size * i + j + 1)) my_hamming++;
    my_manhattan += abs((temp - 1) / (int)my_size - i) + abs((temp - 1) % (int)my_size - j);
}

std::size_t Board::size() const
{
    return my_size;
}

bool Board::is_goal() const
{
    return my_hamming == 0;
}

unsigned Board::hamming() const
{
    return my_hamming;
}

unsigned Board::manhattan() const
{
    return my_manhattan;
}

std::string Board::to_string() const
{
    std::string str;
    for (size_t i = 0; i < my_board.size(); i++)
    {
        for (size_t j = 0; j < my_board.size() - 1; j++)
            str += std::to_string(my_board[i][j]) + ' ';
        str += std::to_string(my_board[i][my_board.size() - 1]) + '\n';
    }
    return str;
}

bool Board::is_solvable() const
{
    size_t inv = 0;
    size_t not_occupied_manhattan = my_size * 2 - not_occupied.first - 2 - not_occupied.second;
    std::vector<int> find_inv;
    for (size_t i = 0; i < my_size; i++)
        for (size_t j = 0; j < my_size; j++)
            find_inv.push_back(my_board[i][j] == 0 ? my_size * my_size : my_board[i][j]);
    for (size_t i = 0; i < find_inv.size(); i++)
        for (size_t j = i + 1; j < find_inv.size(); j++)
            if (find_inv[i] != 0 && find_inv[j] != 0 && find_inv[i] > find_inv[j]) inv++;
    return inv % 2 == (not_occupied_manhattan % 2);
}

const std::vector<unsigned> & Board::operator [] (const std::size_t i) const
{
    return my_board[i];
}

std::vector<Board> Board::find_neighbors()
{
    std::vector<Board> neighbors;
    if (not_occupied.first > 0)
        neighbors.push_back(update({not_occupied.first - 1, not_occupied.second}));
    if (not_occupied.first + 1 < my_size)
        neighbors.push_back(update({not_occupied.first + 1, not_occupied.second}));
    if (not_occupied.second > 0)
        neighbors.push_back(update({not_occupied.first, not_occupied.second - 1}));
    if (not_occupied.second + 1 < my_size)
        neighbors.push_back(update({not_occupied.first, not_occupied.second + 1}));
    return neighbors;
}


Board Board::update(const std::pair<unsigned, unsigned> & b) 
{
    auto ans = my_board;
    unsigned tmp = ans[not_occupied.first][not_occupied.second];
    ans[not_occupied.first][not_occupied.second] = std::move(ans[b.first][b.second]);
    ans[b.first][b.second] = std::move(tmp);
    return  Board(std::move(ans));
}




