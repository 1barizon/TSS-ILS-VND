#include "BucketSystem.h"
#include <iostream>
#include <algorithm>

BucketSystem::BucketSystem(int num_nodes, int initial_max_degree)
    : n(num_nodes), 
      bucket_of(num_nodes, -1), 
      pos(num_nodes, -1),
      min_bucket(-1),
      max_bucket(-1) {
    if (initial_max_degree >= 0) {
        buckets.resize(initial_max_degree + 1);
    }
}

void BucketSystem::insert(int node, int degree) {
    assert(node >= 0 && node < n);
    assert(degree >= 0);
    assert(bucket_of[node] == -1);  // não pode estar em outro bucket

    // Resize se necessário
    if (degree >= (int)buckets.size()) {
        buckets.resize(degree + 1);
    }

    // Inserir nó no final do bucket
    bucket_of[node] = degree;
    pos[node] = (int)buckets[degree].size();
    buckets[degree].push_back(node);

    // Atualizar min/max
    if (min_bucket == -1 || degree < min_bucket) {
        min_bucket = degree;
    }
    if (max_bucket == -1 || degree > max_bucket) {
        max_bucket = degree;
    }
}

void BucketSystem::remove(int node) {
    assert(node >= 0 && node < n);

    int b = bucket_of[node];
    if (b < 0) return;  // Não está em nenhum bucket

    int idx = pos[node];
    int last = buckets[b].back();

    // Swap com último
    buckets[b][idx] = last;
    pos[last] = idx;
    buckets[b].pop_back();

    // Marcar como não no bucket
    bucket_of[node] = -1;
    pos[node] = -1;

    // Atualizar bounds se bucket ficou vazio
    if (buckets[b].empty()) {
        updateBounds();
    }
}


void BucketSystem::move(int node, int old_degree, int new_degree) {
    assert(node >= 0 && node < n);
    assert(old_degree == bucket_of[node]);

    if (old_degree == new_degree) return;  // Nada a fazer

    // Remove do bucket antigo
    remove(node);

    // Insere no bucket novo
    insert(node, new_degree);
}

const std::vector<int>& BucketSystem::getBucket(int degree) const {
    if (degree < 0 || degree >= (int)buckets.size()) {
        static const std::vector<int> empty;
        return empty;
    }
    return buckets[degree];
}

int BucketSystem::getBucketSize(int degree) const {
    if (degree < 0 || degree >= (int)buckets.size()) return 0;
    return (int)buckets[degree].size();
}

int BucketSystem::getTotalNodes() const {
    int total = 0;
    for (const auto& bucket : buckets) {
        total += (int)bucket.size();
    }
    return total;
}

void BucketSystem::updateBounds() {
    // Encontrar novo min_bucket
    min_bucket = -1;
    for (int d = 0; d < (int)buckets.size(); ++d) {
        if (!buckets[d].empty()) {
            min_bucket = d;
            break;
        }
    }

    // Encontrar novo max_bucket
    max_bucket = -1;
    for (int d = (int)buckets.size() - 1; d >= 0; --d) {
        if (!buckets[d].empty()) {
            max_bucket = d;
            break;
        }
    }
}

void BucketSystem::printBuckets() const {
    std::cout << "BucketSystem state (min=" << min_bucket << ", max=" << max_bucket << "):\n";
    for (int d = 0; d < (int)buckets.size(); ++d) {
        if (!buckets[d].empty()) {
            std::cout << "  Bucket[" << d << "]: ";
            for (int node : buckets[d]) {
                std::cout << node << " ";
            }
            std::cout << "\n";
        }
    }
}

void BucketSystem::clear() {
    buckets.clear();
    std::fill(bucket_of.begin(), bucket_of.end(), -1);
    std::fill(pos.begin(), pos.end(), -1);
    min_bucket = -1;
    max_bucket = -1;
}