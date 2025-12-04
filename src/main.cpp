#include <iostream>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"
#include <optional>

int main(){
    std::cout << "Carregando Grafo ..."<< std::endl;
    Graph g("data/clean_data/football.txt");
    g.calcRequisitos(0.5);
    Propagate evaluator(g);
    std::cout << "grafo carregado" <<"Nodes: "<< g.getN() << ", Vertices = " << g.getE() << std::endl; 
    std::cout << "Rodando Guloso..." << std::endl;
    std::optional<std::vector<int>> initial_opt = std::nullopt;
    std::vector<int> solucaoInicial = LocalSearch::Guloso(g.getN(), 0.5, g, evaluator, initial_opt); 
    std::cout << "Tamanho da solucao: " << solucaoInicial.size() << std::endl;

    if(evaluator.isSolution(solucaoInicial)){
        std::cout << "solucao valida!" << std::endl;
    }

    return 0;

}
