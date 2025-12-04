#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

# include <vector>
#include "Graph.h"
#include "Propagate.h"
#include <optional>
namespace LocalSearch
{
    std::vector<int> Guloso(int n, float alpha,Graph& graph, Propagate& evaluator, std::optional<vector<int>> & actualSolution);

    std::vector<std::pair<int, int>> calculateResidualDegree(int n, Graph& graph, std::vector<int>& solution, Propagate& evaluator, bool ascending = true);
    
} // namespace LocalSearch


#endif