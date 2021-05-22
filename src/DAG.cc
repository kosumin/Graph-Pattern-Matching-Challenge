/**
 * @file DAG.cc
 *
 */

#include "../include/DAG.h"
#include <queue>

DAG::DAG(const Graph &G) {
    int size = G.GetNumVertices();
    child.resize(size);
    parent.resize(size);
    Vertex rt = 0;
    std::queue<Vertex> q;
    for (Vertex v = 0; v < G.GetNumVertices(); v++) {
        if (G.GetDegree(v) > G.GetDegree(rt)) rt = v;
    }
    root = rt;
    q.push(root);
    Vertex u;

    while(!q.empty()) {
        u = q.front();
        q.pop();

        for (size_t i = G.GetNeighborStartOffset(u); i < G.GetNeighborEndOffset(u); i++) {
            Vertex v = G.GetNeighbor(i);
            std::set<Vertex>::iterator it = parent[u].find(v);
            if (it == parent[u].end()) {

                child[u].insert(v);

                parent[v].insert(u);

                q.push(v);
            }
        }
    }
}

Vertex DAG::GetChild(Vertex v, size_t i) {
    std::set<Vertex>::iterator it = child[v].begin();
    for (size_t j = 0; j < i; j++, it++);
    return *it;
}

Vertex DAG::GetParent(Vertex v, size_t i) {
    std::set<Vertex>::iterator it = parent[v].begin();
    for (size_t j = 0; j < i; j++, it++);
    return *it;
}

DAG::~DAG() {}
