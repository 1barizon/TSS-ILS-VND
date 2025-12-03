#include "LocalSearch.h"
#include <algorithm>
#include "Propagate.h"

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
    
} // namespace LocalSearch
