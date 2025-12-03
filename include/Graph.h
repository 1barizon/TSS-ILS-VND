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
    void calc_requisitos(double alpha);
    int get_requisito(int u) const;
};

#endif