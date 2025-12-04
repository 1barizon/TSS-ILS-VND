#ifndef BUCKET_SYSTEM_H
#define BUCKET_SYSTEM_H

#include <vector>
#include <cassert>


class BucketSystem {
private:
    int n;
    int min_bucket;
    int max_bucket;
    std::vector<std::vector<int>> buckets;
    std::vector<int> bucket_of;
    std::vector<int> pos;

public:
    //construtor
    BucketSystem(int numNodes, int initialMaxDegree = 0);

    // Inserir nó em um bucket específico
    void insert(int node, int degree);

    // Remover nó do bucket
    void remove(int node);

    // Mover nó de um grau para outro
    void move(int node, int old_degree, int new_degree);

    // Getter: lista de nós com um grau específico
    const std::vector<int>& getBucket(int degree) const;

    // Getter: em qual bucket o nó está?
    int getBucketOf(int node) const { return bucket_of[node]; }

    // Getter: qual é a posição do nó no seu bucket?
    int getPos(int node) const { return pos[node]; }

    // Getter: bucket não-vazio mínimo
    int getMinBucket() const { return min_bucket; }

    // Getter: bucket não-vazio máximo
    int getMaxBucket() const { return max_bucket; }

    // Recompute min/max buckets (após remoções)
    void updateBounds();

    // Tamanho do bucket
    int getBucketSize(int degree) const;

    // Número de nós em todos os buckets
    int getTotalNodes() const;

    // Debug: imprimir estado dos buckets
    void printBuckets() const;

    // Limpar tudo
    void clear();


};




#endif