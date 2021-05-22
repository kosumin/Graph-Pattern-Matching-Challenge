/**
 * @file DAG.h
 *
 */


#ifndef DAG_H_INCLUDED
#define DAG_H_INCLUDED


#include "common.h"
#include "graph.h"

class DAG {
public:
    explicit DAG(const Graph &G);
    ~DAG();

    inline Vertex GetRoot() const;

    inline size_t GetChildNum(Vertex v) const;
    inline size_t GetParentNum(Vertex v) const;
    Vertex GetChild(Vertex v, size_t i);
    Vertex GetParent(Vertex v, size_t i);

private:
    Vertex root;
    std::vector<std::set<Vertex>> child;
    std::vector<std::set<Vertex>> parent;

};

inline Vertex DAG::GetRoot() const { return root; }

inline size_t DAG::GetChildNum(Vertex v) const { return child[v].size(); }

inline size_t DAG::GetParentNum(Vertex v) const { return parent[v].size(); }

#endif // DAG_H_INCLUDED
