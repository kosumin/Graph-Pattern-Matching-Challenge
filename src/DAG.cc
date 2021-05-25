/**
 * @file DAG.cc
 *
 */

#include "../include/DAG.h"
#include "../include/candidate_set.h"
#include <queue>
#include <algorithm>

const Graph *gg;
const CandidateSet *gcs;

bool lf_compare(const Vertex& v1, const Vertex& v2)
{
    if (gg->GetLabelFrequency(gg->GetLabel(v1)) < gg->GetLabelFrequency(gg->GetLabel(v2)))
    /*if (gcs->GetCandidateSize(v1) < gcs->GetCandidateSize(v2))*/
        return true;
    else
        return false;
}

bool deg_compare(const Vertex& v1, const Vertex& v2)
{
    /*if (gg->GetLabelFrequency(gg->GetLabel(v1)) == gg->GetLabelFrequency(gg->GetLabel(v2)) && gg->GetDegree(v1) > gg->GetDegree(v2))*/
    if (gg->GetDegree(v1) > gg->GetDegree(v2))
         return true;
    else
        return false;
}

bool lf_deg_compare(const Vertex& v1, const Vertex& v2)
{
    if (gg->GetLabelFrequency(gg->GetLabel(v1)) < gg->GetLabelFrequency(gg->GetLabel(v2)))
        return true;
    else if (gg->GetLabelFrequency(gg->GetLabel(v1)) == gg->GetLabelFrequency(gg->GetLabel(v2)) && gg->GetDegree(v1) > gg->GetDegree(v2))
    /*if (gcs->GetCandidateSize(v1) == gcs->GetCandidateSize(v2) && gg->GetDegree(v1) > gg->GetDegree(v2))*/
         return true;
    else
        return false;
}

DAG::DAG(const Graph &G, const CandidateSet &cs, const Graph &data) {
    gg = &G;
    gcs = &cs;
    int size = G.GetNumVertices();
    int visited[size];
    for (size_t i = 0; i < size; i++) visited[i] = 0;
    child.resize(size);
    parent.resize(size);
    Vertex rt = 0;
    int count = 0;
    for (int i = 0; i < data.GetNumVertices(); i++)
    {
        if (data.GetLabel(i) == G.GetLabel(rt) && data.GetDegree(i) >= G.GetDegree(rt))
            count++;
    }
    size_t min_size = count / G.GetDegree(rt);
    std::vector<Vertex> q;
    for (Vertex v = 1; v < size; v++) {
    count = 0;
    for (int i = 0; i < data.GetNumVertices(); i++)
    {
        if (data.GetLabel(i) == G.GetLabel(v) && data.GetDegree(i) >= G.GetDegree(v))
            count++;
    }
    printf("%d : %d %d\n", v, count / G.GetDegree(v), G.GetDegree(v));
        if (min_size > count / G.GetDegree(v)) {
            rt = v;
            min_size = count /G.GetDegree(v);
       }
    }
    //rt = 19;
    root = rt;
    q.push_back(root);
    Vertex u;
    visited[rt] = 1;
    while (true) {
        std::sort(q.begin(), q.end(), deg_compare);
        std::sort(q.begin(), q.end(), lf_compare);
        count = q.size();
        if (count == 0)
            break;
        while(count > 0) {
            u = q[0];
            q.erase(q.begin());
            count--;
        
            for (size_t i = G.GetNeighborStartOffset(u); i < G.GetNeighborEndOffset(u); i++)
            {
                Vertex v = G.GetNeighbor(i);
                
            
            std::set<Vertex>::iterator it = parent[u].find(v);
            if (it == parent[u].end()) {
                printf("p: %d c: %d cd: %d lf: %d\n", u, v, G.GetDegree(v), G.GetLabelFrequency(G.GetLabel(v)));
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
}

DAG::~DAG() {}
