#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

# include <vector>
#include "Graph.h"
#include "Propagate.h"
#include <optional>
namespace LocalSearch
{
    std::vector<bool> Guloso(int n, float alpha,Graph& graph, Propagate& evaluator, std::optional<std::vector<bool>>& actualSolution);
    std::vector<bool> shake(Graph& graph, Propagate& evaluator, std::vector<bool> solution, float intensity);
    std::vector<bool> removeFix(Graph& graph, Propagate& evaluator, std::vector<bool> solution);
    std::vector<bool> addRemove(Graph& graph, Propagate& evaluator, std::vector<bool> solution);
} // namespace LocalSearch


#endif