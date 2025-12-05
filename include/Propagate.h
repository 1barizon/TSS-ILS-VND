#ifndef PROPAGATE_H
#define PROPAGATE_H

#include "Graph.h"
#include <vector>

class Propagate {
private:
    const Graph& graph; // referencia para o grafo

    std::vector<int> activeNeighborCount;
    std::vector<int> propagationQueue;

public:
    std::vector<bool> isActive;
    Propagate(const Graph& g);
    int evaluate(const std::vector<bool>& solution);
    bool isSolution(const std::vector<bool>& solution);
};


#endif