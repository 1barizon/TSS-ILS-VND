#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>

using namespace std;

class Graph {
private:
    int nodes; // numero de nos
    int edges; // numero de arestas
    vector<vector<int>> adj; // matriz de adjacencia
    vector<int> reqs;

public:
    // construtor
    Graph(const std::string& filepath);
 
    // getters
    int getN() const;
    int getE() const;

    const std::vector<int>& getNeighbors(int u) const;
    // requisitos
    void calcRequisitos(double alpha);
    int getRequisito(int u) const;
};

#endif