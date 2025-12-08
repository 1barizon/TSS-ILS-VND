#include <iostream>
#include <optional>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "Graph.h"
#include "Propagate.h"
#include "LocalSearch.h"
#include "ILS.h"

int main(){
    
    const int runs = 10;
    std::string dataPath = "data/clean_data/";

    // Load all graphs and sort by size
    std::vector<std::pair<std::string, Graph>> graphs;

    for(const auto& entry : std::filesystem::directory_iterator(dataPath)){
        if(entry.is_regular_file() && entry.path().extension() == ".txt"){
            graphs.emplace_back(entry.path().filename().string(), Graph(entry.path().string()));
        }
    }

    // Sort graphs by number of vertices (ascending)
    std::sort(graphs.begin(), graphs.end(), [](const auto& a, const auto& b){
        return a.second.getN() < b.second.getN();
    });

    // Create logs directory if it doesn't exist
    std::filesystem::create_directories("../logs");
    
    std::ofstream logFile("../logs/results.txt");

    // Process from smallest to largest graph
    for(auto& [filename, g] : graphs){
        g.calcRequisitos(0.5);
        Propagate eval(g);
        int bestOverall = 0;
        double sumCosts = 0.0;
        
        std::cout << "Processing: " << filename << std::endl;
        

        for(int i = 0; i < runs; i++){
            ILS ils(g, eval, 1000, 0.3f, 200);
            auto best = ils.run();
            int cost = ils.getBestCost();
            
            sumCosts += cost;
            if(i == 0 || cost < bestOverall){
                bestOverall = cost;
            }
            std::cout << bestOverall <<std::endl;
        
        }
        double avgCost = sumCosts / runs;
       
        logFile << filename << " | Best: " << bestOverall << " | Average: " << avgCost << std::endl;
        std::cout << filename << " | Best: " << bestOverall << " | Average: " << avgCost << std::endl;
        
    
    }

    logFile.close();
    return 0;
}
