#include "Graph.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

Graph::Graph(const string& filepath){
    ifstream file(filepath);
    if (!file.is_open()){
        cerr << "Erro: nao foi possivel abrir" << filepath << endl;
        exit(EXIT_FAILURE);
    }

    file >> nodes >> edges;
    adj.resize(nodes);

    int u,v;
    while (file >> u >> v){
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    file.close(); 
}

int Graph::getN() const {
    return nodes;
}

int Graph::getE() const {
    return edges;
}

const vector<int>& Graph::getNeighbors(int u) const{
    return adj[u];
}

void Graph::calcRequisitos(double alpha){
    reqs.resize(nodes);
    for (int i = 0 ; i < nodes; i++){
        int degree = adj[i].size();
        int k = std::ceil(degree*alpha);
        if (k < 1 && degree>0) k = 1;

        reqs[i] = k;
    }
}

int Graph::getRequisito(int u) const{
    return reqs[u];
}
