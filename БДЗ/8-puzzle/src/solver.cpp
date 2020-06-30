#include "solver.h"
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <utility>

Solver::Solver(const Board &board)
{
    if (board.size() == 1 || board.size() == 0)
    {
        m_moves.push_back(board);
        return ;
    }
    if (board.is_solvable())
    {
        auto cmp_frontier = [](Board f, Board s)
        {
            return f.hamming() + f.manhattan() > s.hamming() + s.manhattan();
        };
        std::priority_queue<Board, std::vector<Board>, decltype(cmp_frontier)> frontier(cmp_frontier);
        frontier.push({board});
        std::map<Board, Board> parent;
        std::map<Board, unsigned> cur_cost;
        std::set<Board> visited;
        cur_cost.emplace(board, 0);
        while (!frontier.empty())
        {
            auto cur_board = frontier.top();
            if (cur_board.is_goal())
                break;
            frontier.pop();
            visited.insert(cur_board);
            for (auto & neighbor : cur_board.find_neighbors())
            {
                unsigned new_cost;
                new_cost = cur_cost[cur_board] + neighbor.hamming() + neighbor.manhattan();
                if (!visited.count(neighbor) || new_cost < cur_cost[neighbor])
                {
                    cur_cost[neighbor] = new_cost;
                    parent[neighbor] = cur_board;
                    frontier.push(std::move(neighbor));
                }
            }
        }
        if (frontier.top().is_goal())
        {
            auto cur = frontier.top();
            m_moves.push_back(cur);
            while (m_moves.back() != board)
            {
                cur = parent[cur];
                m_moves.push_back(cur);
            }
            std::reverse(m_moves.begin(), m_moves.end());
        }
    }
}

std::size_t Solver::moves() const
{
    if (m_moves.size() == 0) return 0;
        return m_moves.size() - 1;
}

