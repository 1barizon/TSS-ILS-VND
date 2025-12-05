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
    std::vector<int> solucaoInicial = LocalSearch::Guloso(g.getN(), 0.4, g, evaluator, initial_opt); 
    std::cout << "Tamanho da solucao: " << solucaoInicial.size() << std::endl;
    std::vector<int> newSolution = LocalSearch::Swap(g, evaluator, solucaoInicial);
    std::cout << "Tamanho da nova solucao: " << newSolution.size() << std::endl;
    std::cout << "Solucao Inicial: ";
    for (int node : solucaoInicial) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    std::cout << "Nova Solucao:    ";
    for (int node : newSolution) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

  
    return 0;

}
