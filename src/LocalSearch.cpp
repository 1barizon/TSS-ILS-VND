#include "LocalSearch.h"
#include <algorithm>
#include "Propagate.h"
#include "Graph.h"
#include <utility>
namespace LocalSearch
{
    std::vector<int> Guloso(int n, Propagate& evaluator){
        std::vector<int> solution;
        std::vector<bool> isSelected(n, false);
        int currentActivated = 0;

        while(currentActivated<n){
            int bestNode = -1;
            int MaxActivated = -1;

            for(int i = 0; i < n; i++){
                if(isSelected[i]) continue;

                solution.push_back(i);
                int activated = evaluator.evaluate(solution);
                if(activated > MaxActivated){
                    MaxActivated = activated;
                    bestNode = i;
                }
                solution.pop_back();
            }
            if (bestNode != -1){
                solution.push_back(bestNode);
                isSelected[bestNode] = true;
                currentActivated = MaxActivated;
            }else {
                break;
            }
        }
        return solution;
    }    

    std::vector<int> calculateResidualDegree(int n, Graph& graph, std::vector<int>& solution, Propagate& evaluator){
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

        
        std::sort(residualDegree.begin(), residualDegree.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b){
            if (a.first != b.first) return a.first > b.first;
            return a.second < b.second;
        });

        std::vector<int> order;
        order.reserve(n);
        for (const auto& p : residualDegree) order.push_back(p.second);
        return order;
    }
    
} // namespace LocalSearch
