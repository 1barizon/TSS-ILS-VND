#ifndef PROPAGATE_H
#define PROPAGATE_H

#include "Graph.h"
#include <vector>

class Propagate {
private:
    const Graph& graph; // referencia para o grafo

    std::vector<int> activeNeighborCount;
    std::vector<bool> isActive;
    std::vector<int> propagationQueue;

public:

    Propagate(const Graph& g);
    int evaluate(const std::vector<int>& solution);
    bool isSolution(const std::vector<int>& solution);
};


#endif