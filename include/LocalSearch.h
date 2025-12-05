#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

# include <vector>
#include "Graph.h"
#include "Propagate.h"
#include <optional>
namespace LocalSearch
{
    std::vector<uint8_t> Guloso(int n, float alpha,Graph& graph, Propagate& evaluator, std::optional<std::vector<uint8_t>>& actualSolution);
    std::vector<uint8_t> shake(Graph& graph, Propagate& evaluator, std::vector<uint8_t> solution, float intensity);
    std::vector<uint8_t> removeFix(Graph& graph, Propagate& evaluator, std::vector<uint8_t> solution);
    std::vector<uint8_t> addRemove(Graph& graph, Propagate& evaluator, std::vector<uint8_t> solution);
} // namespace LocalSearch


#endif