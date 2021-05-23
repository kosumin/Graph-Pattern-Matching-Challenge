/**
 * @file DAG.cc
 *
 */

#include "../include/DAG.h"
#include "../include/candidate_set.h"
#include <queue>

DAG::DAG(const Graph &G, const CandidateSet &cs) {
    int size = G.GetNumVertices();
    child.resize(size);
    parent.resize(size);
    Vertex rt = 0;
    size_t min_size = cs.GetCandidateSize(rt);
    std::queue<Vertex> q;
    for (Vertex v = 1; v < size; v++) {
        if (min_size > cs.GetCandidateSize(v)) {
            rt = v;
            min_size = cs.GetCandidateSize(v);
        }
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

DAG::~DAG() {}
