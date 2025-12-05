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
    ILS ils(g, eval, 1000, 0.3f, 200);
    auto best = ils.run();
    int bestSize = ils.getBestCost();
    std::cout << "melhor tamanho: "<<bestSize << std::endl;

    return 0;


}
