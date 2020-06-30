#pragma once

#include <string>
#include <vector>
#include <random>

class Board
{
public:
    static Board create_goal(unsigned size);

    Board();

    explicit Board(unsigned size);

    explicit Board(std::vector<std::vector<unsigned>> data);

    std::size_t size() const;

    bool is_goal() const;

    unsigned hamming() const;

    unsigned manhattan() const;

    std::string to_string() const;

    bool is_solvable() const;

    friend bool operator == (const Board & lhs, const Board & rhs)
    {
        if (lhs.size() != rhs.size()) return false;
        return lhs.my_board == rhs.my_board;
    }

    friend bool operator < (const Board & lhs, const Board & rhs)
    {
        for (size_t i = 0; i < lhs.size(); i++)
            for (size_t j = 0; j < lhs[i].size(); j++)
                if (lhs[i][j] != rhs[i][j]) return lhs[i][j] < rhs[i][j];
        return false;
    }

    friend bool operator != (const Board & lhs, const Board & rhs)
    { return !(lhs == rhs);}

    const std::vector<unsigned> & operator [] (std::size_t i) const;

    friend std::ostream & operator << (std::ostream & out, const Board & board)
    { return out << board.to_string(); }

    std::vector<Board> find_neighbors();

private:
    std::vector<std::vector<unsigned>> my_board;
    size_t my_size;
    unsigned my_hamming = 0;
    int my_manhattan = 0;
    std::pair<unsigned, unsigned> not_occupied = std::make_pair(0, 0);
    void find_man_ham(unsigned, unsigned, int);

    Board update(const std::pair<unsigned, unsigned> &);
};
