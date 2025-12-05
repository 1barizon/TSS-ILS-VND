#include <iostream>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"
#include <optional>
#include "ILS.h"

int main(){
    
    Graph g("data/clean_data/socfb-Brandeis99.txt");
    g.calcRequisitos(0.5);
    Propagate eval(g);
    // ILS ils(g, eval, 1000, 0.2f, 100);
    // auto best = ils.run();
    // int bestSize = ils.getBestCost();
    // std::cout << "melhor tamanho: "<<bestSize << std::endl;
	std::optional<std::vector<bool>> none = std::nullopt;
    std::vector<bool> solucao = LocalSearch::Guloso(g.getN(), 0.5, g, eval, none);
    std::vector<bool> shake_sol = LocalSearch::shake(g, eval, solucao, 0.2);
    int solucao_size = 0;
    int shake_size = 0;
    for(int i =0; i<g.getN(); i++){
        if(solucao[i]) solucao_size++;
        if(shake_sol[i]) shake_size++; 
    } 
    std::cout << "solucao guloso:" << solucao_size << std::endl;
    std::cout << "solucao shake:" <<   shake_size << std::endl;
    return 0;


}
