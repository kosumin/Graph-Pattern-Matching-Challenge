/**
 * @file DAG.cc
 *
 */

#include "../include/DAG.h"

const Graph *gg;
const CandidateSet *gcs;

bool cand_compare(const Vertex& v1, const Vertex& v2)
{
    if (gcs->GetCandidateSize(v1) < gcs->GetCandidateSize(v2))
        return true;
    else
        return false;
}

bool deg_compare(const Vertex& v1, const Vertex& v2)
{
    if (gg->GetDegree(v1) > gg->GetDegree(v2))
         return true;
    else
        return false;
}

DAG::DAG(const Graph &G, const CandidateSet &cs, const Graph &data) {
    gg = &G;
    gcs = &cs;
    
    int size = G.GetNumVertices();
    int visited[size];
    for (int i = 0; i < size; i++) visited[i] = 0;
    
    child.resize(size);
    parent.resize(size);

    Vertex rt = 0;
    int cur_size;
    size_t min_size = SIZE_MAX;
    std::vector<Vertex> q;
    
    // Select a root vertex with the smallest (refined candidate size / degree) value
    for (Vertex v = 0; v < size; v++) { 
        
        cur_size = cs.GetCandidateSize(v) / G.GetDegree(v);
          
        if (min_size >= cur_size) {
            rt = v;
            min_size = cur_size;
       } 
    }
    
    root = rt;
    q.push_back(root);
    visited[rt] = 1;
    
    // Build DAG 
    while (q.size() != 0)
    {
        // Sort the visited vertex list q in decreasing order of degree 
        std::sort(q.begin(), q.end(), deg_compare);
        // Sort the visited vertex list q in increasing order of candidate size
        std::sort(q.begin(), q.end(), cand_compare);
        
        // Select current vertex
        Vertex u = q[0];
        q.erase(q.begin());

        // Add unvisited neighbor vertices to the list q
        for (size_t i = G.GetNeighborStartOffset(u); i < G.GetNeighborEndOffset(u); i++)
        {
                Vertex v = G.GetNeighbor(i);
               
                std::set<Vertex>::iterator it = parent[u].find(v);
                if (it == parent[u].end()) {
                    child[u].insert(v);
                    parent[v].insert(u);
                        
                    if (visited[v] == 0)
                    {
                        visited[v] = 1;
                        q.push_back(v);
                    }
                }
         }

    }
   
}

DAG::~DAG() {}
