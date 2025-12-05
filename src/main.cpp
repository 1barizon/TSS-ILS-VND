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
    std::optional<std::vector<bool>> initial_opt = std::nullopt;
    std::vector<bool> solucaoInicial = LocalSearch::Guloso(g.getN(), 0.4, g, evaluator, initial_opt); 
    
    int solucaoInicialSize = 0;
    for (int i = 0; i < g.getN(); ++i) {
        if (solucaoInicial[i]) solucaoInicialSize++;
    }
    std::cout << "Tamanho da solucao: " << solucaoInicialSize << std::endl;
    
    std::vector<bool> newSolution = LocalSearch::addRemove(g, evaluator, solucaoInicial);
    int newSolutionSize = 0;
    for (int i = 0; i < g.getN(); ++i) {
        if (newSolution[i]) newSolutionSize++;
    }
    std::cout << "Tamanho da nova solucao: " << newSolutionSize << std::endl;
    
    std::cout << "Solucao Inicial: ";
    for (int i = 0; i < g.getN(); ++i) {
        if (solucaoInicial[i]) std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Nova Solucao:    ";
    for (int i = 0; i < g.getN(); ++i) {
        if (newSolution[i]) std::cout << i << " ";
    }
    std::cout << std::endl;

  
    return 0;

}
