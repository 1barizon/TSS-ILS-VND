#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

# include <vector>
#include "Graph.h"
#include "Propagate.h"
#include <optional>
namespace LocalSearch
{
    std::vector<int> Guloso(int n, float alpha,Graph& graph, Propagate& evaluator, std::optional<vector<int>> & actualSolution);
    std::vector<int> shake(Graph& graph, Propagate& evaluator, std::vector<int> solution, float intensity);
    std::vector<int> Swap(Graph& graph, Propagate& evaluator, std::vector<int> solution);
    std::vector<int> removeFix(Graph& graph, Propagate& evaluator, std::vector<int> solution);
    std::vector<int> addRemove(Graph& graph, Propagate& evaluator, std::vector<int> solution);
} // namespace LocalSearch


#endif