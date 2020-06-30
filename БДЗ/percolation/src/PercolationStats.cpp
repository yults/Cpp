#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include "PercolationStats.h"
#include "Percolation.h"

PercolationStats::PercolationStats(size_t dimension, size_t trials) : n(dimension),
t(trials), mean(0), standard_deviation(0)
{
    execute();
}

double PercolationStats::get_mean() const
{
    return mean;
}

double PercolationStats::get_standard_deviation() const
{
    return standard_deviation;
}

double PercolationStats::get_confidence_low() const
{
    return get_mean() - ((1.96 * get_standard_deviation()) /sqrt(t));
}

double PercolationStats::get_confidence_high() const
{
    return get_mean() + ((1.96 * get_standard_deviation()) / sqrt(t));
}

void PercolationStats::execute()
{
    std::vector<double> x(t);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, n - 1);
    for (size_t i = 0; i < t; i++)
    {
        Percolation percolation(n);
        while(!percolation.has_percolation())
        {
            size_t row = dist(mt);
            size_t column = dist(mt);
            percolation.open(row, column);
        }
        double result = percolation.get_numbet_of_open_cells() / (double)(n * n);
        x[i] = result;
        mean += result;
    }
    mean /= t;
    for(size_t i = 0; i < t; i++)
    {
        standard_deviation += (x[i] - mean) * (x[i] - mean);
    }
    standard_deviation = sqrt(standard_deviation / (t - 1));
}
