#include <iostream>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"

int main(){
    std::cout << "Carregando Grafo ..."<< std::endl;
    Graph g("data/clean_data/ca-CondMat.txt");
    g.calc_requisitos(0.5);
    Propagate evaluator(g);
    std::cout << "grafo carregado" <<"Nodes: "<< g.getN() << ", Vertices = " << g.getE() << std::endl; 
    std::cout << "Rodando Guloso..." << std::endl;
    std::vector<int> solucaoInicial = LocalSearch::Guloso(g.getN(), evaluator); 
    std::cout << "Tamanho da solucao: " << solucaoInicial.size() << std::endl;

    if(evaluator.isSolution(solucaoInicial)){
        std::cout << "solucao valida!" << std::endl;
    }

    return 0;
}
