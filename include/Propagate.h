#ifndef PROPAGATE_H
#define PROPAGATE_H

#include "Graph.h"
#include <vector>
#include <cstdint>

class Propagate {
private:
    const Graph& graph; // referencia para o grafo

    std::vector<int> activeNeighborCount;
    std::vector<int> propagationQueue;

public:
    std::vector<uint8_t> isActive;
    Propagate(const Graph& g);
    int evaluate(const std::vector<uint8_t>& solution);
    bool isSolution(const std::vector<uint8_t>& solution);
};


#endif