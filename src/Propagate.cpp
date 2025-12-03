#include "Propagate.h"
#include <algorithm>


Propagate::Propagate(const Graph& g) : graph(g) {
    int n = graph.getN();
    activeNeighborCount.resize(n);
    isActive.resize(n);
    propagationQueue.reserve(n);
}

int Propagate::evaluate(const std::vector<int>& solution){
    int n = graph.getN();
    std::fill(activeNeighborCount.begin(), activeNeighborCount.end(), 0);
    std::fill(isActive.begin(), isActive.end(), false);
    propagationQueue.clear();

    int totalActive = 0;

    // 1. ativar a semente 
    for(int node : solution) {
        if (!isActive[node]){
            isActive[node] = true;
            propagationQueue.push_back(node);
            totalActive++;
        }
    }

    // 2. processo de difusao
    int head = 0;
    while(head < propagationQueue.size()){
        int u = propagationQueue[head++];
        for (int v : graph.getNeighbors(u)){
            if (isActive[v]) continue;
            activeNeighborCount[v]++;
            if (activeNeighborCount[v] >= graph.get_requisito(v)){
                isActive[v] = true;
                propagationQueue.push_back(v);
                totalActive++;

            }
        }
    }
    return totalActive;
}

bool Propagate::isSolution(const std::vector<int>& solution){
        return evaluate(solution) == graph.getN();
}