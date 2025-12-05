#ifndef ILS_H
#define ILS_H

#include <functional>
#include <vector>
#include <optional>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"

class ILS {
private:
    Graph &graph;
    Propagate &evaluator;

    std::vector<bool> currentSolution;
    std::vector<bool> bestSolution;
    std::vector<bool> bestOfAll;

    int maxIterations;
    int iterLimit;
    float perturbationStrength;

    std::vector<std::function<std::vector<bool>(Graph &, Propagate &, std::vector<bool>)>> neighborhoods;

    int solutionSize(const std::vector<bool> &solution) const;
    std::vector<bool> runVND(const std::vector<bool> &start);

public:
    ILS(Graph &graph, Propagate &evaluator, int iterations = 1000, float strength = 0.1f, int iterLimit = 100);

    std::vector<bool> run();
    const std::vector<bool> &getBestSolution() const;
    int getBestCost() const;
};

#endif