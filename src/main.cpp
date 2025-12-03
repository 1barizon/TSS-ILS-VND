#include <iostream>
#include "Graph.h"

int main(){
    std::cout << "Carregando Grafo ..."<< std::endl;
    Graph g("data/clean_data/amazon0302.txt");
    g.calc_requisitos(0.5);
    std::cout << "grafo carregado" <<"Nodes: "<< g.getN() << ", Vertices = " << g.getE() << std::endl; 
    std::cout << "requisito do no 0 -" << g.get_requisito(0) << endl; 
    return 0;
}
