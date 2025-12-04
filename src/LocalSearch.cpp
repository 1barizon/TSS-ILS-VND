#include "LocalSearch.h"
#include "Propagate.h"
#include "Graph.h"

#include <vector>
#include <utility>
#include <random>
#include <chrono>
#include <algorithm>

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

    // Greedy randomized constructive with efficient incremental residual-degree bookkeeping
    std::vector<int> Guloso(int n, float alpha, Graph& graph, Propagate& evaluator, std::optional<std::vector<int>>& actualSolution){
        static thread_local std::mt19937_64 rng(
            (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
        std::vector<int> solution;
        solution.reserve(n);
        if (actualSolution.has_value() && !actualSolution->empty())
            solution = *actualSolution;
        evaluator.evaluate(solution);
        std::vector<bool> active = evaluator.isActive; 
        std::vector<int> deg(n, 0);
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

        std::vector<std::vector<int>> buckets(max_deg + 1);
        std::vector<int> bucket_of(n, -1), pos(n, -1); // pos in bucket, bucket_of = degree index
        for (int i = 0; i < n; ++i) {
            if (!active[i]) {
                int d = deg[i];
                if (d >= (int)buckets.size()) buckets.resize(d + 1);
                bucket_of[i] = d;
                pos[i] = (int)buckets[d].size();
                buckets[d].push_back(i);
            }
        }

        auto removeFromBucket = [&](int node) {
            int b = bucket_of[node];
            if (b < 0) return;
            int idx = pos[node];
            int last = buckets[b].back();
            buckets[b][idx] = last;
            pos[last] = idx;
            buckets[b].pop_back();
            bucket_of[node] = -1;
            pos[node] = -1;
        };

        auto moveNodeDegree = [&](int node, int oldd, int newd) {
            if (oldd == newd) return;
            // remove from old
            int b = bucket_of[node];
            if (b >= 0) {
                int idx = pos[node];
                int last = buckets[b].back();
                buckets[b][idx] = last;
                pos[last] = idx;
                buckets[b].pop_back();
            }
            // ensure buckets big enough
            if (newd >= (int)buckets.size()) buckets.resize(newd + 1);
            bucket_of[node] = newd;
            pos[node] = (int)buckets[newd].size();
            buckets[newd].push_back(node);
        };

        int cur_max = (int)buckets.size() - 1;
        while (cur_max >= 0 && buckets[cur_max].empty()) --cur_max;
        int cur_min = 0;
        while (cur_min <= cur_max && buckets[cur_min].empty()) ++cur_min;

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
                for (int node : buckets[d]) {
                    if (!active[node]) rcl.push_back(node);
                }
            }

            if (rcl.empty()) {
                for (int d = cur_min; d <= cur_max; ++d) {
                    for (int node : buckets[d]) if (!active[node]) rcl.push_back(node);
                }
                if (rcl.empty()) break;
            }

            std::uniform_int_distribution<size_t> dist(0, rcl.size() - 1);
            int selected = rcl[dist(rng)];
            solution.push_back(selected);

            // mark selected active and remove from bucket
            if (!active[selected]) {
                active[selected] = true;
                removeFromBucket(selected);
            }

            // Update neighbors' degrees (each neighbor loses 1 inactive neighbor when 'selected' is activated)
            const std::vector<int> neighbors = graph.getNeighbors(selected);
            for (int nb : neighbors) {
                if ((unsigned)nb >= (unsigned)n) continue;
                if (active[nb]) continue;
                int oldd = deg[nb];
                int newd = oldd > 0 ? oldd - 1 : 0;
                deg[nb] = newd;
                moveNodeDegree(nb, oldd, newd);
            }

            // Recompute cur_max/cur_min efficiently
            while (cur_max >= 0 && (cur_max >= (int)buckets.size() || buckets[cur_max].empty())) --cur_max;
            cur_min = 0;
            while (cur_min <= cur_max && buckets[cur_min].empty()) ++cur_min;

            // Call evaluator to let propagation happen (if it activates additional nodes, sync them)
            evaluator.evaluate(solution);
            if (evaluator.isActive != active) {
                // find newly activated nodes and update buckets/deg accordingly
                for (int i = 0; i < n; ++i) {
                    if (active[i] == false && evaluator.isActive[i]) {
                        // i became active via propagation
                        active[i] = true;
                        // remove from bucket if present
                        removeFromBucket(i);
                        // neighbors lose one inactive neighbor
                        for (int nb : graph.getNeighbors(i)) {
                            if ((unsigned)nb >= (unsigned)n) continue;
                            if (active[nb]) continue;
                            int oldd = deg[nb];
                            int newd = oldd > 0 ? oldd - 1 : 0;
                            deg[nb] = newd;
                            moveNodeDegree(nb, oldd, newd);
                        }
                    }
                }
                // refresh bounds
                while (cur_max >= 0 && (cur_max >= (int)buckets.size() || buckets[cur_max].empty())) --cur_max;
                cur_min = 0;
                while (cur_min <= cur_max && buckets[cur_min].empty()) ++cur_min;
            }
        }

        // Final evaluator sync (ensure evaluator state matches returned solution)
        evaluator.evaluate(solution);
        return solution;
    }
} // namespace LocalSearch
