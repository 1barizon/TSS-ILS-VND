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
    std::vector<std::pair<int,int>> calculateResidualDegree(int n, Graph& graph, const std::vector<int>& solution, const Propagate& evaluator, bool ascending){
        const std::vector<bool>& actives = evaluator.isActive;
        std::vector<std::pair<int,int>> residualDegree;
        residualDegree.reserve(n);

        for (int i = 0; i < n; ++i){
            int degree = 0;
            const std::vector<int> neighbors = graph.getNeighbors(i);
            for (int neighbor : neighbors) {
                if ((unsigned)neighbor < (unsigned)n && !actives[neighbor]) {
                    ++degree;
                }
            }
            residualDegree.emplace_back(degree, i);
        }
        if (ascending) {
            std::sort(residualDegree.begin(), residualDegree.end(),
                      [](const std::pair<int,int>& a, const std::pair<int,int>& b){
                          return a.first < b.first;
                      });
        } else {
            std::sort(residualDegree.begin(), residualDegree.end(),
                      [](const std::pair<int,int>& a, const std::pair<int,int>& b){
                          return a.first > b.first;
                      });
        }
        return residualDegree;
    }

    std::vector<int> Guloso(int n, float alpha, Graph& graph, Propagate& evaluator, std::optional<std::vector<int>>& actualSolution){
        // gerar numero aleatorio
        static thread_local std::mt19937_64 rng(
            (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        // solucao tamanho max = n reservado
        std::vector<int> solution;
        solution.reserve(n);
        // caso uma pre solucao seja passada
        if (actualSolution.has_value() && !actualSolution->empty())
            solution = *actualSolution;
        // evalueate para qusar isActive e ficar atualizado 
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
            solution.push_back(selected);

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
            int cur_min = bs.getMinBucket();
            int cur_max = bs.getMaxBucket();
            evaluator.evaluate(solution);
            if (evaluator.isActive != active) {
                // encontrar novos ativos para atualzir
                for (int i = 0; i < n; ++i) {
                    std::cout << std::endl;
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
                cur_min = bs.getMinBucket();
            }
        }

        // Final evaluator sync (ensure evaluator state matches returned solution)
        evaluator.evaluate(solution);
        return solution;
    }
} // namespace LocalSearch
