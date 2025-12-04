#include "LocalSearch.h"
#include <algorithm>
#include "Propagate.h"
#include "Graph.h"
#include <utility>
#include <cstdlib>
#include <ctime>
#include <optional>

namespace LocalSearch
{
    std::vector<std::pair<int,int>> calculateResidualDegree(int n, Graph& graph, std::vector<int>& solution, Propagate& evaluator, bool ascending);

    std::vector<int> Guloso(int n, float alpha, Graph& graph,Propagate& evaluator, std::optional<std::vector<int>>& actualSolution){
        srand(time(0));
        std::vector<int> solution;
        solution.reserve(n);
        if (actualSolution.has_value() && !actualSolution->empty())
            solution = *actualSolution;
        std::vector<bool> isSelected(n, false);
        int currentActivated = 0;
        std::vector<bool> actives  = evaluator.isActive;
        while(!evaluator.isSolution(solution)){
            std::vector<std::pair<int, int>> candidatesDegree = calculateResidualDegree(n , graph, solution, evaluator, false);
            std::vector<std::pair<int, int>> cl;
            
            for (const auto& p : candidatesDegree) {
                if (!actives[p.second]) {
                    cl.push_back(p);
                }
            }

            if (cl.empty()) {
                break;
            }

            // grau limite para entrar
            int max_deg = cl.front().first;
            int min_deg = cl.back().first;
            int degree_limit = static_cast<int>(min_deg + alpha * (max_deg - min_deg));

            std::vector<int> rcl;
            for (const auto& no : cl) {
                if (no.first >= degree_limit)
                    rcl.push_back(no.second);
            }

            if (rcl.empty()) {
                for (const auto& no : cl) rcl.push_back(no.second);
            }

            int randomIndex = (rcl.empty() ? 0 : rand() % rcl.size());
            int selected = rcl[randomIndex];
            solution.push_back(selected);
            actives[selected] = true;
            currentActivated++;
            evaluator.evaluate(solution);
        }
        return solution;
    }    

    std::vector<std::pair<int,int>> calculateResidualDegree(int n, Graph& graph, std::vector<int>& solution, Propagate& evaluator, bool ascending){
        int activated = evaluator.evaluate(solution);
        std::vector<bool> actives = evaluator.isActive;
        std::vector<std::pair<int, int>> residualDegree;
        residualDegree.resize(n);
        for (int i = 0; i < n; i++){
            int degree = 0;
            std::vector<int> neighbors = graph.getNeighbors(i);
            for (int neighbor : neighbors) {
                if (neighbor >= 0 && neighbor < n && !actives[neighbor]) {
                    degree++;
                }
            }
            residualDegree[i] = std::make_pair(degree, i);
        }


        if(ascending){
            std::sort(residualDegree.begin(), residualDegree.end(),
                      [](const std::pair<int,int>& a, const std::pair<int,int>& b){
                          return a.first < b.first;
                      });
        }
        if(!ascending){
            std::sort(residualDegree.begin(), residualDegree.end(),
                      [](const std::pair<int,int>& a, const std::pair<int,int>& b){
                          return a.first > b.first;
                      });
        }
        
        
        return residualDegree;
    }
    
} // namespace LocalSearch
