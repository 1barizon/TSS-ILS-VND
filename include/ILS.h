#ifndef ILS_H
#define ILS_H

#include <functional>
#include <vector>
#include <optional>
#include <cstdint>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"

class ILS {
private:
    Graph &graph;
    Propagate &evaluator;

    std::vector<uint8_t> currentSolution;
    std::vector<uint8_t> bestSolution;
    std::vector<uint8_t> bestOfAll;

    int maxIterations;
    int iterLimit;
    float perturbationStrength;

    std::vector<std::function<std::vector<uint8_t>(Graph &, Propagate &, std::vector<uint8_t>)>> neighborhoods;

    int solutionSize(const std::vector<uint8_t> &solution) const;
    std::vector<uint8_t> runVND(const std::vector<uint8_t> &start);

public:
    ILS(Graph &graph, Propagate &evaluator, int iterations = 1000, float strength = 0.1f, int iterLimit = 100);

    std::vector<uint8_t> run();
    const std::vector<uint8_t> &getBestSolution() const;
    int getBestCost() const;
};

#endif