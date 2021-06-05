/**
 * @file DAG.h
 *
 */


#ifndef DAG_H_INCLUDED
#define DAG_H_INCLUDED


#include "common.h"
#include "graph.h"
#include "candidate_set.h"

class DAG {
public:
    explicit DAG(const Graph &G, const CandidateSet &cs, const Graph &D);
    ~DAG();

    inline Vertex GetRoot() const;

    inline size_t GetChildNum(Vertex v) const;
    inline size_t GetParentNum(Vertex v) const;
    inline Vertex GetChild(Vertex v, size_t i);
    inline Vertex GetParent(Vertex v, size_t i);

private:
    Vertex root;
    std::vector<std::set<Vertex>> child;
    std::vector<std::set<Vertex>> parent;

};

inline Vertex DAG::GetRoot() const { return root; }

inline size_t DAG::GetChildNum(Vertex v) const { return child[v].size(); }

inline size_t DAG::GetParentNum(Vertex v) const { return parent[v].size(); }

inline Vertex DAG::GetChild(Vertex v, size_t i) {
    std::set<Vertex>::iterator it = child[v].begin();
    for (size_t j = 0; j < i; j++, it++);
    return *it;
}

inline Vertex DAG::GetParent(Vertex v, size_t i) {
    std::set<Vertex>::iterator it = parent[v].begin();
    for (size_t j = 0; j < i; j++, it++);
    return *it;
}

#endif // DAG_H_INCLUDED
