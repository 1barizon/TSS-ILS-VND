#include "LocalSearch.h"
#include "Propagate.h"
#include "Graph.h"
#include "BucketSystem.h"

#include <vector>
#include <utility>
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

namespace LocalSearch
{
    std::vector<bool> Guloso(int n, float alpha, Graph& graph, Propagate& evaluator, std::optional<std::vector<bool>>& actualSolution){
        // gerar numero aleatorio
        static thread_local std::mt19937_64 rng(
            (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        // solucao binaria: solution[i] = true se o nó i está na solução
        std::vector<bool> solution(n, false);
        
        // caso uma pre solucao seja passada
        if (actualSolution.has_value())
            solution = *actualSolution;
        
        // evalueate para usar isActive e ficar atualizado 
        evaluator.evaluate(solution);
        std::vector<bool> active = evaluator.isActive; 
        std::vector<int> deg(n, 0); // cada no vai ter um grau iniciado com zero
        int max_deg = 0;
        for (int i = 0; i < n; ++i) {
            const std::vector<int> neighbors = graph.getNeighbors(i);
            int d = 0;
            for (int nb : neighbors) {
                if ((unsigned)nb < (unsigned)n && !active[nb]) ++d; // quantidade de vizinhos inativos de i
            }
            deg[i] = d;
            if (!active[i] && d > max_deg) max_deg = d;
        }

        BucketSystem bs(n, max_deg);
        for(int i = 0; i < n; ++i){
            if(!active[i]){
                bs.insert(i, deg[i]);
            }
        }


        bs.updateBounds();
        int cur_min = bs.getMinBucket();
        int cur_max = bs.getMaxBucket();

        while (!evaluator.isSolution(solution)) {
            if (cur_max < cur_min) break;  

            int maxd = cur_max;
            int mind = cur_min;
            int degree_limit = static_cast<int>(mind + alpha * (maxd - mind));
            if (degree_limit < 0) degree_limit = 0;
            if (degree_limit > maxd) degree_limit = maxd;

            std::vector<int> rcl;
            rcl.reserve(128);
            for (int d = degree_limit; d <= maxd; ++d) {
                for (int node : bs.getBucket(d)) {
                    if (!active[node]) rcl.push_back(node);
                }
            }

            if (rcl.empty()) {
                for (int d = cur_min; d <= cur_max; ++d) {
                    for (int node : bs.getBucket(d)) if (!active[node]) rcl.push_back(node);
                }
                if (rcl.empty()) break;
            }

            std::uniform_int_distribution<size_t> dist(0, rcl.size() - 1);
            int selected = rcl[dist(rng)];
            solution[selected] = true;

            // marcar como no selecionado e remover de buscket 
            if (!active[selected]) {
                active[selected] = true;
                bs.remove(selected);
            }

            // atualizar os graus dos vizinhos, cada vizinho -1 grau
            const std::vector<int> neighbors = graph.getNeighbors(selected);
            for (int nb : neighbors) {
                if ((unsigned)nb >= (unsigned)n) continue;
                if (active[nb]) continue;
                int oldd = deg[nb];
                int newd = oldd > 0 ? oldd - 1 : 0;
                deg[nb] = newd;
                bs.move(nb, oldd, newd);
            }

            bs.updateBounds();
            evaluator.evaluate(solution);
            if (evaluator.isActive != active) {
                // encontrar novos ativos para atualzir
                for (int i = 0; i < n; ++i) {
                    if (active[i] == false && evaluator.isActive[i]) {
                        // i became active via propagation
                        active[i] = true;
                        // remove from bucket if present
                        bs.remove(i);
                        // neighbors lose one inactive neighbor
                        for (int nb : graph.getNeighbors(i)) {
                            if ((unsigned)nb >= (unsigned)n) continue;
                            if (active[nb]) continue;
                            int oldd = deg[nb];
                            int newd = oldd > 0 ? oldd - 1 : 0;
                            deg[nb] = newd;
                            bs.move(nb, oldd, newd);
                        }
                    }
                }
                // refresh bounds
                bs.updateBounds();
            }
        }

        // Final evaluator sync (ensure evaluator state matches returned solution)
        evaluator.evaluate(solution);
        return solution;
    }


    std::vector<bool> shake(Graph &graph, Propagate &evaluator, std::vector<bool> solution, float intensity)
    {
        std::vector<bool> newSolution = solution;
        static thread_local std::mt19937_64 rng(
            (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        
        int n = graph.getN();
        int solutionSize = 0;
        for (int i = 0; i < n; ++i) {
            if (solution[i]) solutionSize++;
        }
        
        int numNodesToRemove = solutionSize * intensity;
        
        // Coletar nós na solução
        std::vector<int> nodesInSolution;
        for (int i = 0; i < n; ++i) {
            if (newSolution[i]) nodesInSolution.push_back(i);
        }
        
        // Remover nós aleatoriamente
        for(int i = 0; i < numNodesToRemove && !nodesInSolution.empty(); i++){
            std::uniform_int_distribution<int> dist(0, nodesInSolution.size() - 1);
            int randomIdx = dist(rng);
            newSolution[nodesInSolution[randomIdx]] = false;
            nodesInSolution.erase(nodesInSolution.begin() + randomIdx);
        }
        
        // fix it com guloso
        std::optional<std::vector<bool>> opt = newSolution;
        newSolution = Guloso(graph.getN(), 0.0, graph, evaluator, opt);
        return newSolution;
    }


    std::vector<bool> removeFix(Graph &graph, Propagate &evaluator, std::vector<bool> solution)
    {
        std::vector<bool> newSolution = solution;
        int n = graph.getN();
        
        // objetivo: mudar a solucao nao necessariamente para melhor
        std::vector<std::pair<int, int>> degrees; 
        for (int node = 0; node < n; ++node){
            if (solution[node]) {
                int degree = graph.getNeighbors(node).size();
                degrees.push_back(std::make_pair(node, degree));
            }
        }
        
        std::sort(degrees.begin(), degrees.end(), [](const std::pair<int, int>a, const std::pair<int, int>b){
            return a.second < b.second;
        });
        
        for(std::pair<int,int> node : degrees){
            std::vector<bool> sol_ = solution;
            sol_[node.first] = false;
            
            if(!evaluator.isSolution(sol_)){
                std::optional<std::vector<bool>> opt = sol_;
                sol_ = Guloso(graph.getN(), 1.0, graph, evaluator, opt);
                
                int newSize = 0, oldSize = 0;
                for (int i = 0; i < n; ++i) {
                    if (sol_[i]) newSize++;
                    if (newSolution[i]) oldSize++;
                }
                
                if(newSize < oldSize || (newSize == oldSize && sol_ != newSolution)) {
                    newSolution = sol_;
                    break;
                }
            }
        }
        
        return newSolution;
    }


    std::vector<bool> addRemove(Graph &graph, Propagate &evaluator, std::vector<bool> solution)
    {
        std::vector<bool> newSolution = solution;
        int n = graph.getN();
        static thread_local std::mt19937_64 rng(
            (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        
        // Coletar nós fora da solução
        std::vector<int> notInSolution;
        for (int i = 0; i < n; ++i) {
            if (!solution[i]) {
            notInSolution.push_back(i);
            }
        }
        
        if (notInSolution.empty()) return newSolution;
        
        // Adicionar um vértice aleatório
        std::uniform_int_distribution<size_t> dist(0, notInSolution.size() - 1);
        int nodeToAdd = notInSolution[dist(rng)];
        newSolution[nodeToAdd] = true;
        
        // Tentar remover o máximo possível
        std::vector<int> nodesInSolution;
        for (int i = 0; i < n; ++i) {
            if (newSolution[i]) nodesInSolution.push_back(i);
        }
        
        std::vector<bool> candidate = newSolution;
        for (int node : nodesInSolution) {
            candidate[node] = false;
            if (evaluator.isSolution(candidate)) {
            newSolution = candidate;
            } else {
            candidate[node] = true;
            }
        }
        
        // Aceitar apenas se a solução diminuir
        int newSize = 0, oldSize = 0;
        for (int i = 0; i < n; ++i) {
            if (newSolution[i]) newSize++;
            if (solution[i]) oldSize++;
        }
        
        if (newSize < oldSize) {
            return newSolution;
        }
        
        return solution;
    }

} // namespace LocalSearch
